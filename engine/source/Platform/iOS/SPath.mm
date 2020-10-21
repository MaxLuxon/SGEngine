#include "SPath.h"
#include "Director.h"

#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#import <CoreServices/CoreServices.h>

std::string SPath::getRessourcePath(){

    NSString * resourcePath = [[NSBundle mainBundle] resourcePath];
    return resourcePath.UTF8String;
    
}

std::string SPath::getDocumentsPath(){

    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    return documentsDirectory.UTF8String;
}


std::string SPath::getLibraryPath(){

    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
    NSString *libraryDirectory = [paths objectAtIndex:0];

    return libraryDirectory.UTF8String;

}


void mycallback( ConstFSEventStreamRef streamRef,  void *clientCallBackInfo,  size_t numEvents, void *eventPaths,
                const FSEventStreamEventFlags eventFlags[], const FSEventStreamEventId eventIds[]){
  
    
//    for (i=0; i<numEvents; i++) {
//        int count;
//        /* flags are unsigned long, IDs are uint64_t */
//        // printf("Change %llu in %s, flags %lu\n", eventIds[i], paths[i], eventFlags[i]);
//    }
    
    char const **paths = (char const**)eventPaths;
    for(int i = 0; i < numEvents; ++i) {
        
//        std::cout << (unsigned long long)eventIds[i] << "\t" <<
//        eventFlags[i] << "\t" <<
//        paths[i] << "\n"
//        ;
        
        sge::Director::getInstance()->getFileUtils()->pathChanged(paths[i]);
    }
    
}


void SPath::setPathsToWatch(const std::vector<std::string>& paths){
    
    std::stringstream ss;
    
    NSMutableArray* array=[[NSMutableArray alloc] init];
    
    for (int i=0; i<paths.size(); i++) {
        
        [array addObject:[NSString stringWithUTF8String:paths[i].c_str()]];
    
    }
    
    //NSArray *pathsToWatch = @[[NSString stringWithUTF8String:path.c_str()]];
//    CFStringRef mypath = (__bridge CFStringRef)[NSString stringWithUTF8String:path.c_str()];
  

    CFArrayRef pathsToWatch = (__bridge CFArrayRef)array;

//    CFStringRef mypath = CFSTR("/Users/SirniciMac/Projects/SGEngine/sge/shader/DeferredLighting.glsl");
//    CFArrayRef pathsToWatch = CFArrayCreate(NULL, (const void **)&mypath, 1, NULL);
    
    FSEventStreamRef stream;
    CFAbsoluteTime latency = 0.01; /* Latency in seconds */
    
    /* Create the stream, passing in a callback */
    stream = FSEventStreamCreate(NULL,
                                 &mycallback,
                                 NULL,
                                 pathsToWatch,
                                 kFSEventStreamEventIdSinceNow, /* Or a previous event ID */
                                 latency,
                                 kFSEventStreamCreateFlagFileEvents
                                 // kFSEventStreamEventFlagItemCreated
                                 // kFSEventStreamCreateFlagNone /* Flags explained in reference */
                                 );
    
    FSEventStreamScheduleWithRunLoop(stream, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    FSEventStreamStart(stream);

    // needs to be released
}
