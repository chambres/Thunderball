#include "EffectMgr.h"

#include <SexyAppFramework/Image.h>
#include <SexyAppFramework/ModVal.h>

#include "Board.h"
#include "PhysObj.h"
#include "Res.h"
#include "ThunderCommon.h"

#include <cmath>

using namespace Sexy;

// FUNCTION: POPCAPGAME1 0x0043e540
Effect::Effect()
{
	mUnk0x44 = NULL;
	mUnk0x48 = NULL;
}

// FUNCTION: POPCAPGAME1 0x00440100
Effect::Effect(const Effect& param_1)
{
	mType = param_1.mType;
	mUnk0xc = param_1.mUnk0xc;
	mUnk0x10 = param_1.mUnk0x10;
	mUnk0x14 = param_1.mUnk0x14;
	mUnk0x18 = param_1.mUnk0x18;
	mUnk0x1c = param_1.mUnk0x1c;
	mUnk0x20 = param_1.mUnk0x20;
	mUnk0x24 = param_1.mUnk0x24;
	mUnk0x28 = param_1.mUnk0x28;
	mUnk0x2c = param_1.mUnk0x2c;
	mUnk0x30 = param_1.mUnk0x30;
	mUnk0x34 = param_1.mUnk0x34;
	mUnk0x4 = param_1.mUnk0x4;
	mUnk0x38 = param_1.mUnk0x38;
	mUnk0x3c = param_1.mUnk0x3c;
	mUnk0x40 = param_1.mUnk0x40;
	mUnk0x4c = param_1.mUnk0x4c;
	mUnk0x50 = param_1.mUnk0x50;
	mUnk0x54 = param_1.mUnk0x54;
	mUnk0x44 = NULL;
	mUnk0x48 = NULL;
}

// FUNCTION: POPCAPGAME1 0x00439bd0
Effect::~Effect()
{
	delete mUnk0x44;
	delete mUnk0x48;
}

// FUNCTION: POPCAPGAME1 0x004471e0
EffectMgr::EffectMgr()
{
}

// SYNTHETIC: POPCAPGAME1 0x0044b550
// Sexy::EffectMgr::`scalar deleting destructor'

// FUNCTION: POPCAPGAME1 0x00447210
EffectMgr::~EffectMgr()
{
}

// FUNCTION: POPCAPGAME1 0x00443720
void EffectMgr::Clear()
{
	for (int i = 0; i < 5; ++i) {
		mEffects[i].clear();
	}
}

// FUNCTION: POPCAPGAME1 0x0044de40
Effect* EffectMgr::AddEffect(EffectType param_1, float param_2, float param_3, int param_4, bool param_5)
{
	int priority = ClampPriority(param_4);
	std::list<Effect>& effects = mEffects[PriorityToList(priority)];
	std::list<Effect>::iterator position = param_5 ? effects.begin() : effects.end();
	std::list<Effect>::iterator it = effects.insert(position, Effect());
	Effect* effect = &*it;
	effect->mUnk0x14 = param_2;
	effect->mType = param_1;
	effect->mUnk0x18 = param_3;
	effect->mUnk0x4 = priority;
	effect->mUnk0x10 = 100;
	return effect;
}

// STUB: POPCAPGAME1 0x0045bc60
Effect* EffectMgr::AddLevelBumper(PhysObj* param_1)
{
	return NULL;
}

// FUNCTION: POPCAPGAME1 0x004514d0
void EffectMgr::AddSpookyBallWrap(float param_1, float param_2, float param_3, float param_4)
{
	int aDuration = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp914,4667", 80);
	int aPriority = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp915,4668", 2);
	if (ModVal(0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp916,4673", 0)) {
		for (int i = 0; i < 2; ++i) {
			Effect* anEffect = AddEffect((EffectType)0x23, param_3, param_4, aPriority, false);
			anEffect->mUnk0x38 = i;
			anEffect->mUnk0x28 = SEXY_PI * 1.5f;
			anEffect->mUnk0x10 = aDuration;
		}
	}

	Effect* anEffect = AddEffect((EffectType)0x23, param_1, param_2, aPriority, false);
	anEffect->mUnk0x28 = SEXY_PI / 2.0f;
	anEffect->mUnk0x10 = aDuration;
	anEffect->mUnk0x38 = param_1 < 325.0f;
}

// FUNCTION: POPCAPGAME1 0x00451310
void EffectMgr::AddSpookyBall(float param_1, float param_2, bool param_3)
{
	int aDuration = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp906,4623", 40);
	int aPriority = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp907,4624", 0);
	float aSpeed = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp908,4625", 0.3f);

	for (int i = 0; i < 4; ++i) {
		Effect* anEffect = AddEffect((EffectType)0x22, param_1, param_2, aPriority, false);
		switch (i) {
		case 0:
			anEffect->mUnk0x20 = -aSpeed;
			break;
		case 1:
			anEffect->mUnk0x20 = aSpeed;
			break;
		case 2:
			anEffect->mUnk0x1c = aSpeed;
			break;
		case 3:
			anEffect->mUnk0x1c = -aSpeed;
			break;
		}
		anEffect->mUnk0x38 = i;
		anEffect->mUnk0x10 = aDuration;
	}

	aDuration = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp909,4642", 100);
	aPriority = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp910,4643", 2);
	int aDelay = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp911,4644", 5);
	int aStartAngle = Rand();
	for (int i = 0; i < 3; ++i) {
		Effect* anEffect = AddEffect((EffectType)0x23, param_1, param_2, aPriority, false);
		anEffect->mUnk0x10 = aDuration;
		anEffect->mUnk0xc = -aDelay;
		anEffect->mUnk0x28 = (float)(
			(double)i * 6.2831854820251465 / 3.0 +
			(double)(aStartAngle % 360) * 3.1415927410125732 / 180.0);
		anEffect->mUnk0x34 = ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp912,4652", 5);
	}

	if (param_3) {
		Effect* anEffect = AddEffect((EffectType)0x24, param_1, param_2, aPriority, false);
		anEffect->mUnk0x10 = ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp913,4659", 10000);
	}
}

// FUNCTION: POPCAPGAME1 0x00451140
void EffectMgr::AddStyleSpin(float param_1, float param_2)
{
	if (GetBoard() == NULL) {
		return;
	}

	int aPriority = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp898,4592", 0);
	int aDuration = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp899,4593", 40);
	int aCount = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp900,4594", 35);
	int aRadius = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp901,4595", 60);
	float aRadiusFloat = (float)aRadius;
	float aCountFloat = (float)aCount;
	int anXCenter = (int)param_1;

	for (int i = 0; i < aCount; ++i) {
		float aRandom = (float)(Rand() % 1000) / 1000.0f;
		float aScale = 1.0f - aRandom * aRandom;
		float anAngle = (float)i;
		anAngle = (float)(anAngle * 2.0f * 3.1415927410125732 / aCountFloat);
		double anXScale =
			(double)ModVal(
				0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp902,4604", 1.5f) *
			aRadiusFloat * aScale;
		float aSin = (float)sin(anAngle);
		float anX = (float)((double)aSin * anXScale + param_1);
		int aYOffset = ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp903,4605", 10);
		double aYCenter = (double)param_2 - aYOffset;
		double aYScale =
			(double)ModVal(
				0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp904,4605", 0.5f) *
			aRadiusFloat * aScale;
		float aCos = (float)cos(anAngle);
		float aY = (float)((double)aCos * aYScale + aYCenter);

		Effect* anEffect = AddEffect((EffectType)0x2e, anX, aY, aPriority, false);
		anEffect->mUnk0x10 = aDuration;
		anEffect->mUnk0x38 = anXCenter;
		anEffect->mUnk0x3c = (int)((double)param_2 - ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp905,4610", 10));
		anEffect->mUnk0x40 = true;
	}
}

// FUNCTION: POPCAPGAME1 0x00450ff0
void EffectMgr::AddJimmy(bool param_1, int param_2, int param_3)
{
	Board* aBoard = GetBoard();
	if (aBoard == NULL) {
		return;
	}

	int anX;
	if (param_1) {
		anX = aBoard->mUnk0x1b0 + ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp893,4559", -40);
	}
	else {
		anX = ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp894,4559", 40) -
			IMAGE_JIMMY->mWidth;
	}
	float anXPos = (float)anX;
	float aY = (float)(aBoard->mUnk0x1b4 + ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp895,4560", -3));
	int aDuration = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp896,4562", 150);
	if (param_2 < 0) {
		param_2 = Rand();
	}
	int aCel = param_2 % 255;

	Effect* anEffect = AddEffect((EffectType)0x1f, anXPos, aY, -1, false);
	anEffect->mUnk0x10 = aDuration;
	anEffect->mUnk0x38 = param_1;
	anEffect->mUnk0x34 = aCel;
	anEffect->mUnk0xc = -param_3;
	anEffect->mUnk0x40 = true;

	int aPriority = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp897,4576", 2);
	anEffect = AddEffect((EffectType)0x1f, anXPos, aY, aPriority, false);
	anEffect->mUnk0x34 = aCel;
	anEffect->mUnk0x38 = param_1;
	anEffect->mUnk0x10 = aDuration;
	anEffect->mUnk0xc = -param_3;
	anEffect->mUnk0x40 = true;
}

// FUNCTION: POPCAPGAME1 0x00450d30
void EffectMgr::AddCatcherGlow(int param_1, bool param_2)
{
	Board* aBoard = GetBoard();
	if (aBoard == NULL) {
		return;
	}

	PhysObj* aCatcher = NULL;
	for (std::list<SmartPtr<PhysObj> >::iterator anItr = aBoard->mUnk0x190.begin();
		 anItr != aBoard->mUnk0x190.end(); ++anItr) {
		PhysObj* anObj = *anItr;
		if (anObj->mUnk0x10 == 1 &&
			anObj->mUnk0x5c.compare(0, anObj->mUnk0x5c.size(), "freeball", 8) == 0) {
			aCatcher = anObj;
			break;
		}
	}
	if (aCatcher == NULL) {
		return;
	}

	Effect* anEffect = AddEffect(
		(EffectType)0x1a, aCatcher->GetXPos(), aCatcher->GetYPos(), 0, false);
	anEffect->mUnk0xc = -param_1;
	anEffect->mUnk0x10 = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp883,4527", 90);
	anEffect->mUnk0x4c = aCatcher;
	if (param_2) {
		anEffect->mUnk0x38 = 1;
	}

	int aCount = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp884,4534", 20);
	for (int i = 0; i < aCount; ++i) {
		double aRandom = 1.0 - (double)(Rand() % 1000) / 1000.0;
		int anX = (int)((double)ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp885,4538", 60) * aRandom);
		int aYRange = ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp888,4539", 40);
		int aY = ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp887,4539", 20) -
			Rand() % aYRange;
		int anXOffset = ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp886,4539", 0);

		anEffect = AddEffect((EffectType)0x1b, (float)(anXOffset + anX), (float)aY, 0, false);
		anEffect->mUnk0x1c = ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp889,4540", 0.01f) *
			(float)anX;
		anEffect->mUnk0x20 = (float)(Rand() % 50 + 50) * ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp890,4541", 0.01f);
		anEffect->mUnk0xc = -param_1;
		int aDurationRange = ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp892,4543", 50);
		anEffect->mUnk0x10 = Rand() % aDurationRange + ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp891,4543", 70);
		anEffect->mUnk0x4c = aCatcher;
		anEffect->mUnk0x38 = Rand() % 3 + 1;
		anEffect->mUnk0x3c = Rand() % 100;
	}
}

// FUNCTION: POPCAPGAME1 0x00450c60
void EffectMgr::AddPyramid(float param_1, float param_2, bool param_3)
{
	Board* aBoard = GetBoard();
	if (aBoard == NULL) {
		return;
	}

	for (std::list<SmartPtr<PhysObj> >::iterator anItr = aBoard->mUnk0x190.begin();
		 anItr != aBoard->mUnk0x190.end(); ++anItr) {
		PhysObj* anObj = *anItr;
		if (!anObj->mUnk0x94.empty() &&
			anObj->mUnk0x94.compare(0, anObj->mUnk0x94.size(), "bumperc", 7) == 0) {
			Effect* anEffect = AddEffect(
				(EffectType)0x17,
				param_1,
				param_2,
				ModVal(0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp881,4497", 2),
				false);
			anEffect->mUnk0x10 = ModVal(
				0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp882,4498", 120);
			anEffect->mUnk0x4c = anObj;
			return;
		}
	}
}

// STUB: POPCAPGAME1 0x00450940
Effect* EffectMgr::AddFeverRainbow(int param_1)
{
	return NULL;
}

// FUNCTION: POPCAPGAME1 0x0044fd20
Effect* EffectMgr::AddRay(float param_1, float param_2)
{
	Effect* effect = AddEffect(
		(EffectType)0x10,
		param_1,
		param_2,
		ModVal(0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp781,4163", 1),
		false);
	effect->mUnk0x10 = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp782,4164", 200);
	int angleRange = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp784,4165", 100);
	effect->mUnk0x30 = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp783,4165", 362) +
		Rand() % angleRange;
	int velocityRange = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp785,4167", 10);
	effect->mUnk0x28 =
		(float)((Rand() % (velocityRange * 2 + 1) - velocityRange) * 3.14159274101257 / 180.0);
	effect->mUnk0x2c = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp786,4169", 0.01f) *
		effect->mUnk0x28;
	return effect;
}

// STUB: POPCAPGAME1 0x0044fe00
Effect* EffectMgr::AddFeverHoleEffect(float param_1, float param_2, int param_3)
{
	return NULL;
}

// STUB: POPCAPGAME1 0x0044fa20
Effect* EffectMgr::AddSpaceBlast(float param_1, float param_2, int param_3)
{
	return NULL;
}

// STUB: POPCAPGAME1 0x0044f830
Effect* EffectMgr::AddZenPowerup(float param_1, float param_2)
{
	return NULL;
}

// STUB: POPCAPGAME1 0x0044efe0
Effect* EffectMgr::AddCannonBlast(float param_1, float param_2, float param_3)
{
	return NULL;
}

// STUB: POPCAPGAME1 0x0044ec30
Effect* EffectMgr::AddFeverPointsBlast(float param_1, float param_2)
{
	return NULL;
}

// STUB: POPCAPGAME1 0x0044e880
Effect* EffectMgr::AddFeverBlast(float param_1, float param_2)
{
	return NULL;
}

// FUNCTION: POPCAPGAME1 0x0044e7f0
Effect* EffectMgr::AddFeverScoreText()
{
	Effect* effect = AddEffect(
		(EffectType)0x15,
		(float)ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp637,3818", 400),
		(float)ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp638,3818", 300),
		ModVal(0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp639,3818", 0),
		false);
	effect->mUnk0x10 = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp640,3819", 100000);
	effect->mUnk0x38 = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp641,3820", 3);
	return effect;
}

// FUNCTION: POPCAPGAME1 0x0044e740
Effect* EffectMgr::AddBubble(float param_1, float param_2)
{
	Effect* effect = AddEffect((EffectType)9, param_1, param_2, 0, false);
	float speeds[6] = {0.67f, 0.83f, 1.0f, 1.07f, 0.76f, 0.93f};
	effect->mUnk0x20 = -speeds[Rand() % 6];
	effect->mUnk0x30 = Rand() % 4;
	if (effect->mUnk0x30 == 3) {
		effect->mUnk0x30 = Rand() % 4;
	}
	effect->mUnk0x10 = 0;
	return effect;
}

// STUB: POPCAPGAME1 0x0044e010
Effect* EffectMgr::AddFlowerSparkles(float param_1, float param_2)
{
	return NULL;
}

// FUNCTION: POPCAPGAME1 0x0044dfc0
Effect* EffectMgr::AddCoinFlip(float param_1, float param_2, bool param_3)
{
	Effect* effect = AddEffect((EffectType)8, param_1, param_2, 0, false);
	effect->mUnk0x10 = 250;
	effect->mUnk0x30 = Rand() % 360;
	effect->mUnk0x34 = param_3;
	return effect;
}

// FUNCTION: POPCAPGAME1 0x0044df70
Effect* EffectMgr::AddMasterBadge(float param_1, float param_2, int param_3)
{
	Effect* effect = AddEffect(
		(EffectType)0x21,
		param_1,
		param_2,
		ModVal(0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp591,3676", 0),
		false);
	effect->mUnk0x10 = 0;
	effect->mUnk0xc = -param_3;
	return effect;
}

// FUNCTION: POPCAPGAME1 0x0044df20
Effect* EffectMgr::AddRibbon(float param_1, float param_2, int param_3)
{
	Effect* effect = AddEffect(
		(EffectType)7,
		param_1,
		param_2,
		ModVal(0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp590,3666", 0),
		false);
	effect->mUnk0x10 = 0;
	effect->mUnk0xc = -param_3;
	return effect;
}

// STUB: POPCAPGAME1 0x0044e1c0
Effect* EffectMgr::AddExtremeFever(float param_1, float param_2, bool param_3, int param_4)
{
	return NULL;
}

// STUB: POPCAPGAME1 0x0045bf10
void EffectMgr::Update(bool param_1)
{
}

// STUB: POPCAPGAME1 0x0045bfb0
void EffectMgr::UpdateSlowMo(float param_1)
{
}

// STUB: POPCAPGAME1 0x00457340
void EffectMgr::SyncState(DataSync& param_1)
{
}

// FUNCTION: POPCAPGAME1 0x00441650
Effect* EffectMgr::GetEffectByType(EffectType param_1)
{
	for (int i = 0; i < 5; ++i) {
		for (std::list<Effect>::iterator it = mEffects[i].begin(); it != mEffects[i].end(); ++it) {
			if (it->mType == param_1) {
				return &*it;
			}
		}
	}
	return NULL;
}

// FUNCTION: POPCAPGAME1 0x00443780
int EffectMgr::EraseAllOfType(EffectType param_1)
{
	int count = 0;
	for (int i = 0; i < 5; ++i) {
		std::list<Effect>::iterator it = mEffects[i].begin();
		while (it != mEffects[i].end()) {
			if (it->mType == param_1) {
				++count;
				it = mEffects[i].erase(it);
			} else {
				++it;
			}
		}
	}
	return count;
}

// FUNCTION: POPCAPGAME1 0x0044dd30
Effect* EffectMgr::SetPriority(Effect* param_1, int param_2)
{
	int priority = ClampPriority(param_2);
	int oldPriority = ClampPriority(param_1->mUnk0x4);
	if (oldPriority == priority) {
		return param_1;
	}

	std::list<Effect>& oldEffects = mEffects[PriorityToList(oldPriority)];
	std::list<Effect>& newEffects = mEffects[PriorityToList(priority)];
	std::list<Effect>::iterator inserted = newEffects.insert(newEffects.end(), *param_1);
	Effect* effect = &*inserted;
	effect->mUnk0x4 = priority;

	for (std::list<Effect>::iterator it = oldEffects.begin(); it != oldEffects.end(); ++it) {
		if (&*it == param_1) {
			oldEffects.erase(it);
			break;
		}
	}
	return effect;
}

// STUB: POPCAPGAME1 0x004573b0
void EffectMgr::Draw(Graphics* param_1, int param_2)
{
}

// FUNCTION: POPCAPGAME1 0x004415c0
void EffectMgr::FinishFeverRainbow()
{
	int duration = ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\EffectMgr.cpp880,4400", 60);
	for (int i = 0; i < 5; ++i) {
		for (std::list<Effect>::iterator it = mEffects[i].begin(); it != mEffects[i].end(); ++it) {
			if (it->mType == (EffectType)0x18) {
				it->mUnk0x10 = it->mUnk0xc + 100;
			} else if (it->mType == (EffectType)0x26) {
				it->mUnk0x10 = it->mUnk0xc + 80;
			} else if (it->mType == (EffectType)0x15) {
				it->mUnk0x10 = it->mUnk0xc + duration;
			}
		}
	}
}

// FUNCTION: POPCAPGAME1 0x0043bc60
int EffectMgr::ClampPriority(int param_1)
{
	if (param_1 < -1) {
		return -1;
	}
	if (param_1 > 3) {
		return 3;
	}
	return param_1;
}

// FUNCTION: POPCAPGAME1 0x004363c0
int EffectMgr::PriorityToList(int param_1)
{
	return param_1 + 1;
}

// STUB: POPCAPGAME1 0x00433850
void EffectMgr::SetCrackPoint(SexyVector2* param_1, int param_2, int param_3, int param_4)
{
}

