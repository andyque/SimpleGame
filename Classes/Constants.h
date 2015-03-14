//
//  Constants.h
//  SimpleGame
//
//  Created by guanghui on 3/14/15.
//
//

#ifndef SimpleGame_Constants_h
#define SimpleGame_Constants_h

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#define BACKGROUND_MUSIC_SFX  "Sounds/background-music-aac.wav"
#define PEW_PEW_SFX           "Sounds/pew-pew-lei.wav"
#else
#define BACKGROUND_MUSIC_SFX  "background-music-aac.mp3"
#define PEW_PEW_SFX           "pew-pew-lei.mp3"
#endif

#endif
