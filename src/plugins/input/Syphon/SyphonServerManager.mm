/* Copyright (c) 2014-2017 "Omnidome" by Michael Winkelmann
undefined
 * Omnidome was created by Michael Winkelmann aka Wilston Oreo (@WilstonOreo)
 *
 * This file is part of Omnidome.
 *
 * Omnidome is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "SyphonServerManager.h"
#include <QDebug>

#import <Syphon/Syphon.h>

// CFNotificationCallback implementation

static void notificationHandler(CFNotificationCenterRef center, void *observer, CFStringRef name, const void *object, CFDictionaryRef userInfo) {
    (static_cast<omni::input::SyphonServerManager *>(observer))->handleNotification(name, userInfo);
}

namespace omni {
  namespace input {
    SyphonServerManager::SyphonServerManager(){
    }

    SyphonServerManager::~SyphonServerManager(){
      if(isSetup()){
        removeObservers();
      }
    }

    bool SyphonServerManager::isValidIndex(int _idx) const {
      return (_idx >= 0) && (_idx < serverList_.size());
    }

    void SyphonServerManager::setup(){
      if(!isSetup()){
        isSetup_ = true;
        addObservers();
        //don't have to do this in OF.  Not sure what's different in the Cinder implementation, but this works.
        refresh(true);
      }
    }

    bool SyphonServerManager::isSetup() const {
      return isSetup_;
    }


    void SyphonServerManager::refresh(bool _isAnnounce){
      ServerList eventArgs;
      for(NSDictionary* serverDescription in [[SyphonServerDirectory sharedDirectory] servers])
      {
        NSString* name = [serverDescription objectForKey:SyphonServerDescriptionNameKey];
        NSString* appName = [serverDescription objectForKey:SyphonServerDescriptionAppNameKey];
        //NSString *uuid = [serverDescription objectForKey:SyphonServerDescriptionUUIDKey];
        //NSImage* appImage = [serverDescription objectForKey:SyphonServerDescriptionIconKey];
        //NSString *title = [NSString stringWithString:appName];

        SyphonServerDescription sy([appName UTF8String],[name UTF8String]);
        if(_isAnnounce){
            if(!serverExists(sy)){
                serverList_.push_back(sy);
                eventArgs.push_back(sy);
            }
        } else {
            eventArgs.push_back(sy);
        }
    }

    if(!_isAnnounce){
        ServerList foundServers = eventArgs;
        eventArgs.clear();
        for (auto& _serverDescription : serverList_) {
            if(std::find(foundServers.begin(), foundServers.end(), _serverDescription) == foundServers.end()){
                eventArgs.push_back(_serverDescription);
            }
        }
        serverList_ = foundServers;
      }

      if(_isAnnounce){
        emit serverAnnounced(eventArgs);
      } else {
        emit serverRetired(eventArgs);
      }
    }

    bool SyphonServerManager::serverExists(SyphonServerDescription const& _server) const{
      for(auto& s : serverList_){
        if(s == _server){
          return true;
        }
      }
      return false;
    }

    SyphonServerDescription SyphonServerManager::getDescription(int _idx) const{
      return serverList_.at(_idx);
    }

    std::vector<SyphonServerDescription> SyphonServerManager::getServerList() const{
      return serverList_;
    }

    void SyphonServerManager::printList() const{
      for(auto& s : serverList_){
          qDebug() <<"serverName: " << s.serverName() <<"\tappName: "<< s.applicationName()<<"\n";
      }
    }

    int SyphonServerManager::size() const{
      return serverList_.size();
    }

    // Unfortunately userInfo is null when dealing with CFNotifications from a Darwin notification center.  This is one of the few non-toll-free bridges between CF and NS.  Otherwise this class would be far less complicated.
    void SyphonServerManager::handleNotification(CFStringRef name, CFDictionaryRef userInfo){
      if((NSString*)name == SyphonServerAnnounceNotification){
          serverAnnouncedRefresh();
        } else if((NSString*)name == SyphonServerUpdateNotification){
        serverUpdatedRefresh();
    } else if((NSString*)name == SyphonServerRetireNotification){
        serverRetiredRefresh();
      }
    }

    void SyphonServerManager::serverUpdatedRefresh() {
    }

    void SyphonServerManager::serverAnnouncedRefresh(){
      refresh(true);
    }

    void SyphonServerManager::serverRetiredRefresh(){
      refresh(false);
    }


    void SyphonServerManager::addObservers(){
      CFNotificationCenterAddObserver
      (CFNotificationCenterGetLocalCenter(),
      this,
        (CFNotificationCallback)&notificationHandler,
        (CFStringRef)SyphonServerAnnounceNotification,
        NULL,
        CFNotificationSuspensionBehaviorDeliverImmediately
      );

      CFNotificationCenterAddObserver
      (
        CFNotificationCenterGetLocalCenter(),
        this,
        (CFNotificationCallback)&notificationHandler,
        (CFStringRef)SyphonServerUpdateNotification,
        NULL,
        CFNotificationSuspensionBehaviorDeliverImmediately
      );

      CFNotificationCenterAddObserver
      (
        CFNotificationCenterGetLocalCenter(),
        this,
        (CFNotificationCallback)&notificationHandler,
        (CFStringRef)SyphonServerRetireNotification,
        NULL,
        CFNotificationSuspensionBehaviorDeliverImmediately
     );
   }

    void SyphonServerManager::removeObservers(){
     CFNotificationCenterRemoveEveryObserver(CFNotificationCenterGetLocalCenter(), this);
    }

    bool SyphonServerManager::CFStringRefToString(CFStringRef src, QString& dest){
      const char *cstr = CFStringGetCStringPtr(src, CFStringGetSystemEncoding());
      if (!cstr)
      {
        CFIndex strLen = CFStringGetMaximumSizeForEncoding(CFStringGetLength(src) + 1,
                                                           CFStringGetSystemEncoding());
        char *allocStr = (char*)malloc(strLen);

        if(!allocStr)
            return false;

        if(!CFStringGetCString(src, allocStr, strLen, CFStringGetSystemEncoding()))
        {
            free((void*)allocStr);
            return false;
        }

        dest = allocStr;
        free((void*)allocStr);

        return true;
      }

      dest = QString(cstr);
      return true;
    }
  }
}
