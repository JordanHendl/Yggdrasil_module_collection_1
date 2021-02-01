/*
 * Copyright (C) 2020 Jordan Hendl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   Test.cpp
 * Author: Jordan Hendl
 *
 * Created on January 30, 2021, 9:32 AM
 */

#include "HttpImageDownload.h"
#include <yggdrasil/http/ImageDownload.h>
#include <iris/data/Bus.h>
#include <iris/log/Log.h>
#include <vector>
#include <map>
#include <mutex>
#include <thread>

static const unsigned VERSION = 1 ;
namespace ygg
{
  struct HttpImageDownloaderData
  {
    ygg::ImageDownloader downloader ; ///< The yggdrasil object to handle downloading the data.
    iris::Bus            bus        ; ///< The bus to communicate over.
    std::string          name       ; ///< The name of the module.
    std::string          url        ; ///< The url to load.
    std::mutex           mutex      ; ///< Mutex to help make sure command buffers arent being set while this object is processing them.
    
    unsigned width() ;
    unsigned height() ;
    unsigned channels() ;
    const unsigned char* bytes() ;
    void setURL( const char* url ) ;
    void setOutputBytesName( const char* name ) ;
    void setOutputWidthName( const char* name ) ;
    void setOutputHeightName( const char* name ) ;
    void setOutputChannelsName( const char* name ) ;

    /** Default constructor.
     */
    HttpImageDownloaderData() ;
  };
  
  
  unsigned HttpImageDownloaderData::width()
  {
    return this->downloader.width() ;
  }
  
  unsigned HttpImageDownloaderData::height()
  {
    return this->downloader.height() ;
  }
  
  unsigned HttpImageDownloaderData::channels()
  {
    return this->downloader.channels() ;
  }

  const unsigned char* HttpImageDownloaderData::bytes()
  {
    return this->downloader.image() ;
  }
  
  void HttpImageDownloaderData::setURL( const char* url )
  {
    this->url = url ;
    iris::log::Log::output( "Module ", this->name.c_str(), " downloading image: ", url ) ;
    this->downloader.download( this->url.c_str() ) ;
  }
  
  void HttpImageDownloaderData::setOutputBytesName( const char* name )
  {
    this->bus.publish( this, &HttpImageDownloaderData::bytes, name ) ;
  }
  
  void HttpImageDownloaderData::setOutputWidthName( const char* name )
  {
    this->bus.publish( this, &HttpImageDownloaderData::width, name ) ;
  }
  
  void HttpImageDownloaderData::setOutputHeightName( const char* name )
  {
    this->bus.publish( this, &HttpImageDownloaderData::height, name ) ;
  }
  
  void HttpImageDownloaderData::setOutputChannelsName( const char* name )
  {
    this->bus.publish( this, &HttpImageDownloaderData::channels, name ) ;
  }

  HttpImageDownloaderData::HttpImageDownloaderData()
  {
    this->name = "" ;
    this->url  = "" ;
  }

  HttpImageDownload::HttpImageDownload()
  {
    this->module_data = new HttpImageDownloaderData() ;
  }

  HttpImageDownload::~HttpImageDownload()
  {
    delete this->module_data ;
  }

  void HttpImageDownload::initialize()
  {
  }

  void HttpImageDownload::subscribe( unsigned id )
  {
    data().bus.setChannel( id ) ;
    data().name = this->name() ;
    
    data().bus.enroll( this->module_data, &HttpImageDownloaderData::setOutputBytesName   , iris::OPTIONAL, this->name(), "::output_bytes"    ) ;
    data().bus.enroll( this->module_data, &HttpImageDownloaderData::setOutputWidthName   , iris::OPTIONAL, this->name(), "::output_width"    ) ;
    data().bus.enroll( this->module_data, &HttpImageDownloaderData::setOutputHeightName  , iris::OPTIONAL, this->name(), "::output_height"   ) ;
    data().bus.enroll( this->module_data, &HttpImageDownloaderData::setOutputChannelsName, iris::OPTIONAL, this->name(), "::output_channels" ) ;
    data().bus.enroll( this->module_data, &HttpImageDownloaderData::setURL               , iris::OPTIONAL, this->name(), "::image_url"       ) ;
  }

  void HttpImageDownload::shutdown()
  {
  }

  void HttpImageDownload::execute()
  {
    data().bus.wait() ;
    
    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) ) ;
    {
      iris::log::Log::output( this->name(), " emitting data." ) ;
      data().bus.emit() ;
    }
  }

  HttpImageDownloaderData& HttpImageDownload::data()
  {
    return *this->module_data ;
  }

  const HttpImageDownloaderData& HttpImageDownload::data() const
  {
    return *this->module_data ;
  }
}

/** Exported function to retrive the name of this module type.
 * @return The name of this object's type.
 */
exported_function const char* name()
{
  return "HttpImageDownloader" ;
}

/** Exported function to retrieve the version of this module.
 * @return The version of this module.
 */
exported_function unsigned version()
{
  return VERSION ;
}

/** Exported function to make one instance of this module.
 * @return A single instance of this module.
 */
exported_function ::iris::Module* make()
{
  return new ygg::HttpImageDownload() ;
}

/** Exported function to destroy an instance of this module.
 * @param module A Pointer to a Module object that is of this type.
 */
exported_function void destroy( ::iris::Module* module )
{
  ygg::HttpImageDownload* mod ;
  
  mod = dynamic_cast<ygg::HttpImageDownload*>( module ) ;
  delete mod ;
}