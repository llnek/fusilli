//  This file was generated by LevelHelper
//  http://www.levelhelper.org
//
//  LevelHelperLoader.h
//  Created by Bogdan Vladu
//  Copyright 2011 Bogdan Vladu. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//  The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//  Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//  This notice may not be removed or altered from any source distribution.
//  By "software" the author refers to this code file and not the application 
//  that was used to generate this file.
//
////////////////////////////////////////////////////////////////////////////////
//
//  Version history
//  ...............
//  v4.7.7 Fixed LH_PTM_RATIO on iphone4 retina (7 aug 2011)
//  v4.8   Better LH_PTM_RATIO estimation that seams to make the physic the same on all devices
//         Changed sprites movement on paths implementation in order to be able to add pause functionality
//         Added pausePathMovement() forSprite() methods
//  THIS MARKS THE END FOR CODE FEATURES FOR LH 1.3, ONLY BUGS WILL BE FIXES IN CODE TILL LH 1.4 becomes available
//  v4.8.1   Fixed a path issue when not using physics and a pause behaviour
//  v4.8.2 Added a few animation methods
//  v4.8.3 Fixed 2 leaks - made releaseAll method public for Custom CCSprite to avoid leaks
//  v4.8.4 Fixed a release issue of parallax node
//  v4.8.5 Added -(CGRect) physicBoundariesRect; -(CGRect) gameWorldSize;
//  v4.8.5.1 converted physicBoundariesRect and gameWorldSize
//  v4.8.6 Fixed level conversion when using Portrait Mode
//         Added -(b2Body*) bodyWithTag:(enum LevelHelper_TAG)tag touchedAtPoint:(CGPoint)point;
//         Fixed mouseJoint when level is converted
//  v4.8.7 NewBodyWithUniqueName has been changed to keep a reference to the new body
//  v4.8.8 added a warning and removed some logs
//  v4.8.9 Fixed an issue where if you removed a sprite that was moving on a path it was crashing
//  v4.9 Removed some logs
////////////////////////////////////////////////////////////////////////////////

#import <Foundation/Foundation.h>
#import "cocos2d.h"
#import "Box2D.h"

enum LevelHelper_TAG 
{ 
	DEFAULT_TAG 	= 0,
	PLAYER 			= 1,
	MONSTER 			= 2,
	LASER 			= 3,
	GROUND 			= 4,
	NUMBER_OF_TAGS 	= 5
};


enum LH_JOINT_TYPE
{
	LH_DISTANCE_JOINT = 0,
	LH_REVOLUTE_JOINT,
	LH_PRISMATIC_JOINT,
	LH_PULLEY_JOINT,
	LH_GEAR_JOINT,
	LH_LINE_JOINT,
	LH_WELD_JOINT,
    LH_UNKNOWN_TYPE
};

struct LHJoint;

@protocol LevelHelperLoaderCustomCCSprite
@optional

-(CCSprite*) spriteFromDictionary:(NSDictionary*)spriteProp;

-(CCSprite*) spriteWithBatchFromDictionary:(NSDictionary*)spriteProp 
								   batchNode:(CCSpriteBatchNode*)batch;

-(void) removeFromBatchNode:(CCSprite*)sprite;


//this 3 methods should be overloaded together
//first one is for physic sprites 
-(void) setCustomAttributesForPhysics:(NSDictionary*)spriteProp 
                       body:(b2Body*)body
                     sprite:(CCSprite*)sprite;
//this second one is for the case where you dont use physics or you have sprites
//with "NO PHYSIC" as physic type
-(void) setCustomAttributesForNonPhysics:(NSDictionary*)spriteProp 
                               sprite:(CCSprite*)sprite;
//the third one is for bezier shapes that are not paths
-(void) setCustomAttributesForBezierBodies:(NSDictionary*)bezierProp 
                                    sprite:(CCSprite*)sprite body:(b2Body*)body;


@end

@interface LevelHelperLoader : NSObject<LevelHelperLoaderCustomCCSprite> {
	
	NSArray* lhSprites;	//array of NSDictionary with keys GeneralProperties (NSDictionary) 
						//and PhysicProperties (NSDictionary)
	NSArray* lhJoints;	//array of NSDictionary
    NSArray* lhParallax;//array of NSDictionary 
    NSArray* lhBeziers; //array of NSDictionary
    NSArray* lhAnims;   //array of NSDictionary

	NSMutableDictionary* ccSpritesInScene;	//key - uniqueSpriteName	value - CCSprite* or NSValue with b2Body*
	NSMutableDictionary* noPhysicSprites;   //key - uniqueSpriteName    value - CCSprite*
	NSMutableDictionary* ccJointsInScene;   //key - uniqueJointName     value - NSValue withPointer of b2Joint*
    NSMutableDictionary* ccParallaxInScene; //key - uniqueParallaxName  value - CCParallaxNode*
    NSMutableDictionary* ccBeziersBodyInScene;  //key - uniqueBezierName    value - NSValue withPointer of b2Body*
    NSMutableDictionary* ccBezierPathsInScene; //key - uniqueBezierName value - NSArray with NSValue with Point;
	NSMutableDictionary* batchNodes;		//key - textureName			value - NSDictionary
    NSMutableDictionary* ccFramesForAnimation;//key uniqueAnimName      value - NSMutableArray of CCSpriteFrame
	NSDictionary* wb; //world boundaries Info
    b2Body* wbBody;
    
	bool addSpritesToLayerWasUsed;
	bool addObjectsToWordWasUsed;
    
    bool convertPhysic;
    CGPoint safeFrame;
    CGRect gameWorldRect;
    CGPoint gravity;
	
    id pathNotifierId;
    SEL pathNotifierSel;
    
    id animNotifierId;
    SEL animNotifierSel;
    bool notifOnLoopForeverAnim;
    
    NSMutableArray* moveOnPathInfo; //NSValue with lhPathInfo*
    
	CCLayer* cocosLayer; //holds pointer to properly release the sprites
    b2World* box2dWorld; //holds pointer to the box2d world passed at addObjectToWorld
    
    bool convertLevel;
    CGPoint convertRatio;
    
}

////////////////////////////////////////////////////////////////////////////////
//                          INITIALIZING METHODS
////////////////////////////////////////////////////////////////////////////////
-(id) initWithContentOfFile:(NSString*)levelFile;

-(id) initWithContentOfFileFromInternet:(NSString*)webAddress;

-(id) initWithContentOfFile:(NSString*)levelFile 
			 levelSubfolder:(NSString*)levelFolder;

//END                        INITIALIZING METHODS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                          CONSTRUCT WORLD METHODS
////////////////////////////////////////////////////////////////////////////////
/*
 //DISCUSSION
 this will create all the b2Body with the attached sprites.
 PLEASE READ THIS - in order to prepare this class for v1.4 I have added in the userData of CCSprite the following NSMutableDictionary
 this is also necessary in order to make follow path work with static physic sprites or else the position will be reset by the physic update
 
 so 
 b2Body     userData -> CCSprite
 CCSprite   userData -> NSMutableDictionary ->  key "UniqueName" returns NSString
 key "AnimName" returns NSString
 key "CurrentFrame" returns NSNumber numberWithInt in order to know at what frame in the animation you are (only updates on nextFrameForSpriteWithUniqueName and prevFrameForSpriteWithUniqueName)
 key "Body" returns NSValue -> use: b2Body* body = (b2Body)[value pointerValue];
 
 */
-(void) addObjectsToWorld:(b2World*)world cocos2dLayer:(CCLayer*)cocosLayer;

-(void) addSpritesToLayer:(CCLayer*)cocosLayer; //NO PHYSICS
//END                      CONSTRUCT WORLD METHODS
////////////////////////////////////////////////////////////////////////////////

/*
 DISCUSSION:
 Please call this method just before box2d update (the for loop from the tick function)
 NEEDED FOR
 - parallax scrolling
 - follow path to work correctly on sprites with static/kynematic body
 */
-(void) update:(ccTime) dt;


////////////////////////////////////////////////////////////////////////////////
//                          HELPER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
//DISCUSSION - all this methods will fail if you change the userData of a CCSprite*
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//this methods will return nil if fail
+(NSString*) uniqueNameForSprite:(CCSprite*)sprite;
+(NSString*) uniqueNameForBody:(b2Body*)body;
+(NSString*) uniqueNameForJoint:(b2Joint*)joint;

//value must be a NSObject type
+(void) setCustomValue:(id)value withKey:(NSString*)key onSprite:(CCSprite*)sprite;
+(id) customValueWithKey:(NSString*)key forSprite:(CCSprite*)sprite;
//------------------------------------------------------------------------------
//if fail will return -1
+(int) tagForSprite:(CCSprite*)sprite;
//if fail will return -1
+(int) tagForBody:(b2Body*)body;
//------------------------------------------------------------------------------
+(b2Body*) bodyForSprite:(CCSprite*)sprite;
//if fail will return nil
+(CCSprite*) spriteForBody:(b2Body*)body;
//------------------------------------------------------------------------------
//this method will return nil if fail
+(NSString*) animationNameOnBody:(b2Body*)body;
//this method will return nil if fail
+(NSString*) animationNameOnSprite:(CCSprite*)sprite;
//------------------------------------------------------------------------------
//this method will return -1 if fail
+(int) currentFrameOnSprite:(CCSprite*)sprite;
//this method will return -1 if fail
+(int) currentFrameOnBody:(b2Body*)body;
//------------------------------------------------------------------------------
//this method will return -1 if fail
+(int) tagForJoint:(b2Joint*)joint;
//this method will return LH_UNKNOWN_TYPE if fail
+(enum LH_JOINT_TYPE) typeForJoint:(b2Joint*)joint;
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//END                        HELPER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                          BOX2D HELPER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
//DISCUSSION - this was added in order to simulate the physics the same on all 
//              devices
//------------------------------------------------------------------------------
//you should use this methods because in some cases CC_CONTENT_SCALE_FACTOR() 
//is not the same as the one used by LH (eg when converting from iPhone to iPad)

/* this should be called before initWithContentOfFile */
+(void) setMeterRatio:(float)ratio; //default is 32.0f
+(float) meterRatio; //same as pointsToMeterRatio - provided for simplicity as static method

-(float) pixelsToMeterRatio;
-(float) pointsToMeterRatio;

-(b2Vec2) pixelToMeters:(CGPoint)point; //Cocos2d point to Box2d point
-(b2Vec2) pointsToMeters:(CGPoint)point; //Cocos2d point to Box2d point

-(CGPoint) metersToPoints:(b2Vec2)vec; //Box2d point to Cocos2d point
-(CGPoint) metersToPixels:(b2Vec2)vec; //Box2d point to Cocos2d pixels
//END                        BOX2D HELPER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                          CONVERSION METHODS
////////////////////////////////////////////////////////////////////////////////
//you will have to call this before initWithContentOfFile if you dont want retina
//[LevelHelperLoader useRetinaOnIpad:false];
//call this on Mac also with true to use retina
+(void) useRetinaOnIpad:(bool)value;

//this will convert level to the used devide - safe frame must be the size of the
//device resolution inside LevelHelper - by default convertLevel is TRUE
//only call this method do deactivate the convert level option
-(void) convertLevel:(bool)value;

-(CGPoint) convertRatio; //return the conver ratio for current level

-(void) updateConversionRatio; //helper method for Scene Tester
+(void) setCustomAlphaOnAll:(float)value; //helper method for Scene Tester

//END                       CONVERSION METHODS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                   RETRIEVAL  METHODS
////////////////////////////////////////////////////////////////////////////////
-(CCSprite*) spriteWithUniqueName:(NSString*)name; 
-(b2Body*) bodyWithUniqueName:(NSString*)name;
//------------------------------------------------------------------------------
//example of use
/*
 NSArray* arr = [kh spritesWithTag:tag];
 for(CCSprite* spr in arr){ ... }
*/
-(NSArray*) spritesWithTag:(enum LevelHelper_TAG)tag;
//example of use
/*
NSArray* arr = [lh bodiesWithTag:YOUR TAG];
 for(NSValue* val in arr)
 {
    b2Body* body = [val pointerValue];
    ...
 }
*/
-(NSArray*) bodiesWithTag:(enum LevelHelper_TAG)tag;
//------------------------------------------------------------------------------
-(b2Joint*) jointWithUniqueName:(NSString*)name;

/*
 returns NSMutableArray containing NSValue valueWithPoints:b2Joint
 //Example of use
 for(NSValue* val in arrayReturned)
 {
 b2Joint* joint = (b2Joint*)[val pointerValue];
 }
 */
-(NSArray*) jointsWithTag:(enum LevelHelper_TAG)tag;
//------------------------------------------------------------------------------
-(CCSpriteBatchNode*) batchNodeForFile:(NSString*) imageName;
-(NSArray*) batchNodesInLevel;
//------------------------------------------------------------------------------
//this method will return nil for parallaxes with continuous scrolling.
//continuous scrolling is done using a proprietary implementation and does not use
//CCParallaxNode class
-(CCParallaxNode*) paralaxNodeWithUniqueName:(NSString*)uniqueName;
-(NSArray*) spritesInParallaxNodeWithUniqueName:(NSString*)uniqueName;
//END                RETRIEVAL  METHODS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                   NEW  METHODS
////////////////////////////////////////////////////////////////////////////////
//this will create a new sprite in a batch node
-(CCSprite*) newSpriteWithUniqueName:(NSString *)uniqueName;

//this will create a new sprite in the layer - no batch node
-(CCSprite*) newSpriteWithUniqueName:(NSString*)name 
                        cocos2dLayer:(CCLayer*)cocosLayer; 

//example of use
/*
 NSArray* arr = [lh newSpritesWithTag:YOUR TAG cocos2dLayer:self];
 for(CCSprite* spr in arr){...}
 */
-(NSArray*) newSpritesWithTag:(enum LevelHelper_TAG)tag
                        cocos2dLayer:(CCLayer*)cocosLayer;
//------------------------------------------------------------------------------

//the CCSprite associated with the body will NOT use batch nodes
-(b2Body*) newBodyWithUniqueName:(NSString*)name 
						   world:(b2World*)world 
					cocos2dLayer:(CCLayer*)cocosLayer;

//the CCSprite associated with the body will use batch nodes
//this method will also keep a reference to the body so it can be used with mouseJoints
//the uniqueName will be "uniqueName_LH_NEW_BODY_XX" (eg mySprite_LH_NEW_BODY_30)
-(b2Body*) newBodyWithUniqueName:(NSString*)uniqueName 
						   world:(b2World*)world;

//example of use
/*
 NSArray* arr = [lh newBodiesWithTag:YOUR TAG world:world cocos2dLayer:self];
 for(NSValue* val in arr)
 {
 b2Body* body = [val pointerValue];
 ...
 }
 */
-(NSArray*) newBodiesWithTag:(enum LevelHelper_TAG)tag 
							  world:(b2World*)world 
					   cocos2dLayer:(CCLayer*)cocosLayer;
////////////////////////////////////////////////////////////////////////////////
//END                NEW  METHODS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                        REMOVAL  METHODS
////////////////////////////////////////////////////////////////////////////////
-(bool) removeSpriteWithUniqueName:(NSString*)name;
-(bool) removeSprite:(CCSprite*)ccsprite;
-(bool) removeSpritesWithTag:(enum LevelHelper_TAG)tag;
-(bool) removeAllSprites;
//------------------------------------------------------------------------------
-(bool) removeBodyWithUniqueName:(NSString*)name;
-(bool) removeBody:(b2Body*)body;
-(bool) removeBodiesWithTag:(enum LevelHelper_TAG)tag;
-(bool) removeAllBodies;
//------------------------------------------------------------------------------
-(bool) removeJointWithUniqueName:(NSString*)name;
//use this method to remove multiple joints at once
-(void) removeJointsWithTag:(enum LevelHelper_TAG)tag;
-(bool) removeJoint:(b2Joint*) joint;
-(bool) removeAllJoints;
////////////////////////////////////////////////////////////////////////////////
//END                     REMOVAL  METHODS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                        HELPER  METHODS
////////////////////////////////////////////////////////////////////////////////
-(unsigned int) numberOfBatchNodesUsed;
-(bool) isSpriteWithUniqueName:(NSString*)name atPoint:(CGPoint)point;
//point should be given unhandeld - meaning, don't apply x/PTM_RATIO on it
-(bool) isBodyTouched:(b2Body*)body touchPoint:(CGPoint)point;
-(b2Body*) bodyWithTag:(enum LevelHelper_TAG)tag touchedAtPoint:(CGPoint*)point withFingerSize:(int)size;
-(b2Body*) bodyWithTag:(enum LevelHelper_TAG)tag touchedAtPoint:(CGPoint)point;
-(bool) isBodyWithUniqueNameTouched:(NSString*)name touchPoint:(CGPoint)point; 

-(b2MouseJoint*) mouseJointForBody:(b2Body*)body touchPoint:(CGPoint)point;
-(b2MouseJoint*) mouseJointForBodyWithUniqueName:(NSString*)name 
                                      touchPoint:(CGPoint)point;

-(void) setTarget:(CGPoint)point onMouseJoint:(b2MouseJoint*)mouseJoint;
////////////////////////////////////////////////////////////////////////////////
//END                     HELPER  METHODS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                         GRAVITY METHODS
////////////////////////////////////////////////////////////////////////////////
-(bool) isGravityZero;
-(void) createGravity:(b2World*)world;
//END                      GRAVITY METHODS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                    WORLD BOUNDARIES  METHODS
////////////////////////////////////////////////////////////////////////////////
-(bool) hasWorldBoundaries;
-(b2Body*) createWorldBoundaries:(b2World*)world;
-(b2Body*) worldBoundariesBody; //provided for simplicity - you can take this body from createWorldBoundaries method also
-(bool) removeWorldBoundaries;
//this will create wb based on the curret device orientation and size.
+(b2Body*) createFullScreenPhysicBoundaries:(b2World*)world; //provided for simplicity
//END                  WORLD BOUNDARIES  METHODS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                    SCENE INFO  METHODS
////////////////////////////////////////////////////////////////////////////////

-(CGPoint) gameScreenSize; //the device size set in this level
//if no wb defined it will return {{0, 0}, {0, 0}}
-(CGRect) physicBoundariesRect; //the size of physics boundaries
-(CGRect) gameWorldSize; //the size of the game world
//END                 SCENE INFO METHODS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                          PATH METHODS
////////////////////////////////////////////////////////////////////////////////
-(void) moveSpriteWithUniqueName:(NSString*)sprName onPathWithUniqueName:(NSString*)pathUniqueName 
             speed:(float)pathSpeed 
   startAtEndPoint:(bool)startAtEndPoint
          isCyclic:(bool)isCyclic
 restartAtOtherEnd:(bool)restartOtherEnd;

-(void) moveSprite:(CCSprite *)ccsprite onPathWithUniqueName:(NSString*)pathUniqueName 
             speed:(float)pathSpeed 
   startAtEndPoint:(bool)startAtEndPoint
          isCyclic:(bool)isCyclic
 restartAtOtherEnd:(bool)restartOtherEnd;

-(void) pausePathMovement:(bool)pauseStatus ofSpriteWithUniqueName:(NSString*)sprName;

-(void) pausePathMovement:(bool)pauseStatus ofSprite:(CCSprite*)ccsprite;

-(void) releaseAllPathsMovement; //this will cancel all movements on paths
//DISCUSSION
//signature for registered method should be like this: -(void)spriteMoveOnPathEnded:(CCSprite*)spr pathUniqueName:(NSString*)str;
//registration is done like this: [loader registerNotifierOnPathEndPoints:self selector:@selector(spriteMoveOnPathEnded:pathUniqueName:)];
-(void) registerNotifierOnPathEndPoints:(id)obj selector:(SEL)sel;
//END                        PATH METHODS
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                         ANIMATION METHODS
////////////////////////////////////////////////////////////////////////////////
//DISCUSSION
/*
 It is very IMPORTANT that an animation started on a sprite to have the same texture as the sprite (use the same image file)
 */
-(void) startAnimationWithUniqueName:(NSString *)animName onSprite:(CCSprite*)ccsprite;
-(void) startAnimationWithUniqueName:(NSString*)animName onSpriteWithUniqueName:(NSString*)sprName;

-(void) stopAnimationWithUniqueName:(NSString*)animName onSprite:(CCSprite*)ccsprite;
-(void) stopAnimationWithUniqueName:(NSString*)animName onSpriteWithUniqueName:(NSString*)sprName;

//DISCUSSION
/*
 Sometime you may not want to run the animation - but run it frame by frame
 For example when you have a block in multiple states (not braked, half braked, full braked) and you want to change the state when the block is hit
 This two methods work only if the sprite with the assigned animation is in scene.
 */
-(void) nextFrameForSprite:(CCSprite*)ccsprite;
//will reset to first frame when it reaches the end
-(void) nextFrameForSprite:(CCSprite*)ccsprite repeat:(bool)repeat;

-(void) nextFrameForSpriteWithUniqueName:(NSString*)sprName;
//will reset to first frame when it reaches the end
-(void) nextFrameForSpriteWithUniqueName:(NSString*)sprName repeat:(bool)repeat;

-(void) prevFrameForSprite:(CCSprite*)spr;
//will reset to last frame when it reaches the begining
-(void) prevFrameForSprite:(CCSprite*)spr repeat:(bool)repeat;

-(void) prevFrameForSpriteWithUniqueName:(NSString*)sprName;
//will reset to last frame when it reaches the begining
-(void) prevFrameForSpriteWithUniqueName:(NSString*)sprName repeat:(bool)repeat;

//will return -1 if no animation on the body
-(int) numberOfFrameForBody:(b2Body*)body;
//will return -1 if no animation on the sprite
-(int) numberOfFrameForSprite:(CCSprite*)sprite;

//will return false if no animation on sprite or body or not at last frame
-(bool) isBodyAtLastFrame:(b2Body*)body;
-(bool) isSpriteAtLastFrame:(CCSprite*)sprite;


//DISCUSSION
//signature for registered method should be like this: -(void) spriteAnimHasEnded:(CCSprite*)spr animationName:(NSString*)animName
//registration is done like this: [loader registerNotifierOnAnimationEnds:self selector:@selector(spriteAnimHasEnded:animationName:)];
//this will trigger for all type of animations even for the ones controlled by you will next/prevFrameFor...
-(void) registerNotifierOnAnimationEnds:(id)obj selector:(SEL)sel;

//DISCUSSION
/*
 by default the notification on animation end works only on non-"loop forever" animations
 if you want to receive notifications on "loop forever" animations enable this behaviour
 before addObjectsToWorld by calling the following function
 */
-(void) enableNotifOnLoopForeverAnimations;
//END                         ANIMATION METHODS
////////////////////////////////////////////////////////////////////////////////


//needed when deriving this class
-(void) setSpriteProperties:(CCSprite*)ccsprite spriteProperties:(NSDictionary*)spriteProp;

-(void) releaseAll;
////////////////////////////////////////////////////////////////////////////////
@end














