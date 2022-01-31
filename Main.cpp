
#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include "Includes/obfuscate.h"
#include "KittyMemory/MemoryPatch.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Menu.h"

#if defined(__aarch64__)
#include <And64InlineHook/And64InlineHook.hpp>
#else
#include <Substrate/SubstrateHook.h>
#include <Substrate/CydiaSubstrate.h>
#endif


struct My_Patches {

    MemoryPatch level1, level65, superchest, ammo, rent, freeshop, freecraft, collectibles, godmode, silentaim, onehit, crit, jump, speed, points;

} hexPatches;

bool isLevel1;
bool isLevel65;
bool isSuperChest;
bool isAmmo;
bool isRent;
bool isFreeShop;
bool isFreeCraft;
bool isCollectibles;
bool isGodMode;
bool isSilentAim;
bool isOneHit;
bool isCrit;
bool isJump;
bool isSpeed;
bool isPoints;
bool isShotGun = false;
bool isLaser = false;
bool isRocket = false;



void(*old_ShotGun)(void *instance);  //isShotgun
void ShotGun(void *instance) {
    if(instance != NULL) {
        if (isShotGun) { //if Toggle
            *(bool *) ((uint64_t) instance + 0x14A) = true;
        }
    }
    old_ShotGun(instance);
}

void(*old_Laser)(void *instance); //railgun
void Laser(void *instance) {
    if(instance != NULL) {
        if (isLaser) { //if Toggle
            *(bool *) ((uint64_t) instance + 0x15C) = true;
        }
    }
    old_Laser(instance);
}

void(*old_Rocket)(void *instance); //bazooka
void Rocket(void *instance) {
    if(instance != NULL) {
        if (isRocket) { //if Toggle
            *(bool *) ((uint64_t) instance + 0xE7) = true;
        }
    }
    old_Rocket(instance);
}









#define targetLibName OBFUSCATE("libil2cpp.so")



void *hack_thread(void *) {
    LOGI(OBFUSCATE("pthread created"));

    //Check if target lib is loaded
    do {
        sleep(1);
    } while (!isLibraryLoaded(targetLibName));

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

#if defined(__aarch64__)
#else
    hexPatches.level1 = MemoryPatch::createWithHex(targetLibName, //get_CurrentLevel
                                                       string2Offset(OBFUSCATE_KEY("0x2CBB418", '-')),
                                                       OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
    hexPatches.level65 = MemoryPatch::createWithHex(targetLibName, //get_CurrentLevel
                                                       string2Offset(OBFUSCATE_KEY("0x2CBB418", '-')),
                                                       OBFUSCATE("41 00 A0 E3 1E FF 2F E1"));
    hexPatches.superchest = MemoryPatch::createWithHex(targetLibName, //get_ToOpenSpins
                                                    string2Offset(OBFUSCATE_KEY("0x20F6F30", '-')),
                                                    OBFUSCATE("FA 0F A0 E3 1E FF 2F E1"));
    hexPatches.ammo = MemoryPatch::createWithHex(targetLibName,  //get_currentAmmoInClip
                                                       string2Offset(OBFUSCATE_KEY("0x25EDCB0", '-')),
                                                       OBFUSCATE("E7 03 00 E3 1E FF 2F E1"));
    hexPatches.rent = MemoryPatch::createWithHex(targetLibName, //IsAvailableTryGun
                                                       string2Offset(OBFUSCATE_KEY("0x10192DC", '-')),
                                                       OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
    hexPatches.freeshop = MemoryPatch::createWithHex(targetLibName, //ItemPrice
                                                       string2Offset(OBFUSCATE_KEY("0x1C6A67C", '-')),
                                                       OBFUSCATE("00 00 A0 E3 1E FF 2F E1"));
    hexPatches.freecraft = MemoryPatch::createWithHex(targetLibName, //GetDetailsCount
                                                       string2Offset(OBFUSCATE_KEY("0x21CCC60", '-')),
                                                       OBFUSCATE("E7 03 00 E3 1E FF 2F E1"));
    hexPatches.collectibles = MemoryPatch::createWithHex(targetLibName, // :(
                                                       string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                       OBFUSCATE("00 00 A0 E3 1E FF 2F E1"));
    hexPatches.godmode = MemoryPatch::createWithHex(targetLibName, //GetDamageInternal
                                                       string2Offset(OBFUSCATE_KEY("0xF4E658", '-')),
                                                       OBFUSCATE("00 00 A0 E3 1E FF 2F E1"));
    hexPatches.silentaim = MemoryPatch::createWithHex(targetLibName,  //IsHitInDamageRadius
                                                       string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                       OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
    hexPatches.onehit = MemoryPatch::createWithHex(targetLibName, //get_damage
                                                       string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                       OBFUSCATE("00 00 E0 E3 1E FF 2F E1"));
    hexPatches.crit = MemoryPatch::createWithHex(targetLibName, //NextHitCritical
                                                       string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                       OBFUSCATE("01 00 A0 E3 1E FF 2F E1"));
    hexPatches.jump = MemoryPatch::createWithHex(targetLibName, //JumpModifier
                                                       string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                       OBFUSCATE("C2 0A 64 60 00 00 00 02"));
    hexPatches.speed = MemoryPatch::createWithHex(targetLibName, //SpeedModifier
                                                       string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                       OBFUSCATE("C2 0A 64 60 00 00 00 02"));
    hexPatches.points = MemoryPatch::createWithHex(targetLibName, //get_Score
                                                       string2Offset(OBFUSCATE_KEY("0x123456", '-')),
                                                       OBFUSCATE("01 04 A0 E3 1E FF 2F E1"));


    MSHookFunction((void *)getAbsoluteAddress("libil2cpp.so", 0x2A913A8), (void *) ShotGun, (void **) &old_ShotGun);
    MSHookFunction((void *)getAbsoluteAddress("libil2cpp.so", 0x2A913A8), (void *) Laser, (void **) &old_Laser);
    MSHookFunction((void *)getAbsoluteAddress("libil2cpp.so", 0x2A913A8), (void *) Rocket, (void **) &old_Rocket);


    LOGI(OBFUSCATE("Done"));
#endif

    return NULL;
}

//JNI calls
extern "C" {
JNIEXPORT void JNICALL
Java_uk_lgl_MainActivity_Toast(JNIEnv *env, jclass obj, jobject context) {
    MakeToast(env, context, OBFUSCATE("Modded by vox"), Toast::LENGTH_LONG);
}


JNIEXPORT jobjectArray
JNICALL
Java_uk_lgl_modmenu_FloatingModMenuService_getFeatureList(JNIEnv *env, jobject activityObject) {
    jobjectArray ret;

    const char *features[] = {
            OBFUSCATE("Category_The Category"), //Not counted
            OBFUSCATE("Toggle_level 1"),
            OBFUSCATE("Toggle_level 65"),
            OBFUSCATE("Toggle_free super chest"),
            OBFUSCATE("Toggle_unlimited ammo"),
            OBFUSCATE("Toggle_rent hack"),
            OBFUSCATE("Toggle_free shopping"),
            OBFUSCATE("Toggle_free crafting"),
            OBFUSCATE("Toggle_collectibles"),
            OBFUSCATE("Toggle_god mode"),
            OBFUSCATE("Toggle_silent aim"),
            OBFUSCATE("Toggle_one hit kill"),
            OBFUSCATE("Toggle_always critical hit "),
            OBFUSCATE("Toggle_high jump"),
            OBFUSCATE("Toggle_high speed"),
            OBFUSCATE("Toggle_points hack"),
            OBFUSCATE("Toggle_shoot shotgun bullets"),
            OBFUSCATE("Toggle_shoot lasers"),
            OBFUSCATE("Toggle_shoot rockets"),





    };


    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}

JNIEXPORT void JNICALL
Java_uk_lgl_modmenu_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean boolean, jstring str) {

    const char *featureName = env->GetStringUTFChars(featName, 0);
    const char *TextInput;
    if (str != NULL)
        TextInput = env->GetStringUTFChars(str, 0);
    else
        TextInput = "Empty";

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         featureName, value,
         boolean, TextInput);



    switch (featNum) {
        case 0:
            isLevel1 = boolean;
            if (isLevel1) {
                hexPatches.level1.Modify();
            } else {
                hexPatches.level1.Restore();
            }
            break;
        case 1:
            isLevel65 = boolean;
            if (isLevel65) {
                hexPatches.level65.Modify();
            } else {
                hexPatches.level65.Restore();
            }
            break;
        case 2:
            isSuperChest = boolean;
            if (isSuperChest) {
                hexPatches.superchest.Modify();
            } else {
                hexPatches.superchest.Restore();
            }
            break;
        case 3:
            isAmmo = boolean;
            if (isAmmo) {
                hexPatches.ammo.Modify();
            } else {
                hexPatches.ammo.Restore();
            }
            break;
        case 4:
            isRent = boolean;
            if (isRent) {
                hexPatches.rent.Modify();
            } else {
                hexPatches.rent.Restore();
            }
            break;
        case 5:
            isFreeShop = boolean;
            if (isFreeShop) {
                hexPatches.freeshop.Modify();
            } else {
                hexPatches.freeshop.Restore();
            }
            break;
        case 6:
            isFreeCraft = boolean;
            if (isFreeCraft) {
                hexPatches.freecraft.Modify();
            } else {
                hexPatches.freecraft.Restore();
            }
            break;
        case 7:
            isCollectibles = boolean;
            if (isCollectibles) {
                hexPatches.collectibles.Modify();
            } else {
                hexPatches.collectibles.Restore();
            }
            break;
        case 8:
            isGodMode = boolean;
            if (isGodMode) {
                hexPatches.godmode.Modify();
            } else {
                hexPatches.godmode.Restore();
            }
            break;
        case 9:
            isSilentAim = boolean;
            if (isSilentAim) {
                hexPatches.silentaim.Modify();
            } else {
                hexPatches.silentaim.Restore();
            }
            break;
        case 10:
            isOneHit = boolean;
            if (isOneHit) {
                hexPatches.onehit.Modify();
            } else {
                hexPatches.onehit.Restore();
            }
            break;
        case 11:
            isCrit = boolean;
            if (isCrit) {
                hexPatches.crit.Modify();
            } else {
                hexPatches.crit.Restore();
            }
            break;
        case 12:
            isJump = boolean;
            if (isJump) {
                hexPatches.jump.Modify();
            } else {
                hexPatches.jump.Restore();
            }
            break;
        case 13:
            isSpeed = boolean;
            if (isSpeed) {
                hexPatches.speed.Modify();
            } else {
                hexPatches.speed.Restore();
            }
            break;
        case 14:
            isPoints = boolean;
            if (isPoints) {
                hexPatches.points.Modify();
            } else {
                hexPatches.points.Restore();
            }
            break;
        case 15:
            isShotGun = boolean;
            break;
        case 16:
           isLaser = boolean;
            break;
        case 17:
            isRocket = boolean;
            break;

}
}

																	

																										
																	
																	   

						   
									  

								   
		
				 
											  

																								

													 
																																	 
																  
																								  
																						  
											   
																									
																								   
																						   

						  
																																									  
													  

																								 
																																
																					   

																								   

																																	
																  
																								  
																						  
											   
																									
																								   
																						   
												  
								  
								   

						  
															   
																						  
																										 

																																									 
																																			 

																								 
																																
																						  

							
	  

				
 

//No need to use JNI_OnLoad, since we don't use JNIEnv
//We do this to hide OnLoad from disassembler
__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
}

/*
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *globalEnv;
    vm->GetEnv((void **) &globalEnv, JNI_VERSION_1_6);

    return JNI_VERSION_1_6;
}
 */
