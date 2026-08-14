#include "DebugMgr.h"

#include "Ball.h"
#include "Board.h"
#include "LogicMgr.h"
#include "Mover.h"
#include "PegInfo.h"
#include "ThunderCommon.h"
#include "../SexyAppFramework/SexyApp.h"
#include "../SexyAppFramework/WidgetManager.h"

#include <math.h>

using namespace Sexy;

// GLOBAL: POPCAPGAME1 0x00650a38
static int gDebugPegSyncCount = 0;

// FUNCTION: POPCAPGAME1 0x0043ffa0
DebugMgr::DebugMgr(Board* param_1) : mBoard(param_1)
{
	mUnk0x4 = false;
	mUnk0x5 = false;
	mUnk0x6 = false;
	Clear();
	mUnk0x18 = -1;
	mUnk0x14 = -1;
}

// SYNTHETIC: POPCAPGAME1 0x0043e470
// Sexy::DebugMgr::`scalar deleting destructor'

// FUNCTION: POPCAPGAME1 0x00439a80
DebugMgr::~DebugMgr()
{
}

// FUNCTION: POPCAPGAME1 0x00432db0
void DebugMgr::MarkCheat()
{
	mUnk0x20 = true;
}

// FUNCTION: POPCAPGAME1 0x004648e0
void DebugMgr::SyncFlipperState()
{
	MarkCheat();
	LogicMgr* logicMgr = mBoard->mLogicMgr;
	int& powerupCount = logicMgr->mPowerupCount[POWERUP_2][logicMgr->mUnk0x128];
	powerupCount = powerupCount > 0 ? 0 : 1000;
	mUnk0x6 = powerupCount > 0;
	logicMgr->ActivatePowerup(POWERUP_2, powerupCount > 0);
}

// FUNCTION: POPCAPGAME1 0x0054b760
bool DebugMgr::MouseMove(int theX, int theY)
{
	return false;
}

// FUNCTION: POPCAPGAME1 0x0054b760
bool DebugMgr::MouseDrag(int theX, int theY)
{
	return false;
}

// FUNCTION: POPCAPGAME1 0x00432dc0
bool DebugMgr::MouseEnter()
{
	if (mUnk0x4)
		gSexyApp->SetCursor(1);
	return false;
}

// FUNCTION: POPCAPGAME1 0x00432de0
bool DebugMgr::MouseLeave()
{
	if (mUnk0x4)
		gSexyApp->SetCursor(0);
	return false;
}

// FUNCTION: POPCAPGAME1 0x00437350
bool DebugMgr::KeyChar(SexyChar theKey)
{
	return false;
}

// FUNCTION: POPCAPGAME1 0x00437350
bool DebugMgr::MouseWheel(int theDelta)
{
	return false;
}

// FUNCTION: POPCAPGAME1 0x00439ab0
void DebugMgr::UpdateMouseBall()
{
	if (mMouseBall != NULL) {
		float targetX = (float)(gSexyApp->mWidgetManager->mLastMouseX - mBoard->mUnk0x1a8) +
			mMouseOffsetX;
		float targetY = (float)(gSexyApp->mWidgetManager->mLastMouseY - mBoard->mUnk0x1ac) +
			mMouseOffsetY;
		float velocityX = targetX - mMouseBall->mUnk0xec;
		float velocityY = targetY - mMouseBall->mUnk0xf0;
		float magnitude = sqrtf(velocityX * velocityX + velocityY * velocityY);
		float maxVelocity = ModVal(
			0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\DebugMgr.cpp82,148", 10.0f);
		if (magnitude > maxVelocity) {
			velocityX = velocityX * maxVelocity / magnitude;
			velocityY = velocityY * maxVelocity / magnitude;
		}
		mMouseBall->SetAbsPos(targetX, targetY);
		mMouseBall->SetVelocity(velocityX, velocityY);
		mBoard->MarkDirtyFull();
	}
}

// FUNCTION: POPCAPGAME1 0x004414e0
void DebugMgr::SyncFeverState()
{
	MarkCheat();
	mBoard->mLogicMgr->ActivateFreeBall(!mUnk0x5);

	for (std::list<SmartPtr<PhysObj> >::iterator it = mBoard->mUnk0x190.begin();
		 it != mBoard->mUnk0x190.end(); ++it) {
		PhysObj* obj = *it;
		if (obj->mUnk0x5c != "fever") {
			continue;
		}

		obj->SetActive(mUnk0x5);
		if (obj->mMover != NULL) {
			obj->SetMoveUpdateCnt(mUnk0x5 ? obj->mMover->mTime / 2 : 0);
		}
	}
}

// FUNCTION: POPCAPGAME1 0x00440010
Ball* DebugMgr::GetBallAt(float param_1, float param_2)
{
	float closestDistance = 1.0e8f;
	Ball* closestBall = NULL;
	for (std::list<SmartPtr<Ball>>::iterator it = mBoard->mUnk0x19c.begin();
		 it != mBoard->mUnk0x19c.end(); ++it) {
		Ball* ball = *it;
		if (!ball->mUnk0x140) {
			float dx = param_1 - ball->mUnk0xec;
			float dy = param_2 - ball->mUnk0xf0;
			float distance = dx * dx + dy * dy;
			int radius = (int)(ball->mUnk0x13c + 0.5);
			if (radius < 20) {
				radius = 20;
			}
			if (distance < closestDistance && distance <= (float)(radius * radius)) {
				closestBall = ball;
				closestDistance = distance;
			}
		}
	}
	return closestBall;
}

// FUNCTION: POPCAPGAME1 0x00446670
void DebugMgr::SyncNumDebugPegs()
{
	LogicMgr* logicMgr = mBoard->mLogicMgr;
	++gDebugPegSyncCount;
	bool keepPowerupPeg =
		mUnk0x14 < 2 && ((gDebugPegSyncCount % 2) == 0 || mUnk0x14 == 0);

	if (mUnk0x14 >= 0) {
		MarkCheat();
		std::set<SmartPtr<PhysObj> >::iterator it = logicMgr->mUnk0x364.begin();
		while (it != logicMgr->mUnk0x364.end() &&
			   logicMgr->mUnk0x364.size() > (unsigned int)mUnk0x14) {
			PhysObj* obj = *it;
			if (obj->mPegInfo->mPegType == POWERUP && !keepPowerupPeg) {
				if (mUnk0x14 < 2) {
					keepPowerupPeg = true;
				}
				++it;
			} else {
				obj->SetActive(false);
				logicMgr->mUnk0x364.erase(it++);
			}
		}
	}

	if (mUnk0x18 >= 0) {
		MarkCheat();
		std::set<SmartPtr<PhysObj> >::iterator it = logicMgr->mUnk0x358.begin();
		while (it != logicMgr->mUnk0x358.end() &&
			   logicMgr->mUnk0x358.size() > (unsigned int)mUnk0x18) {
			PhysObj* obj = *it;
			obj->SetActive(false);
			logicMgr->mUnk0x358.erase(it++);
		}
	}
}

// FUNCTION: POPCAPGAME1 0x00464930
void DebugMgr::InitLevel()
{
	SyncNumDebugPegs();
	if (mUnk0x5) {
		SyncFeverState();
	}
	if (mUnk0x6) {
		SyncFlipperState();
	}
}

// FUNCTION: POPCAPGAME1 0x0043e4a0
void DebugMgr::Clear()
{
	mMouseBall = NULL;
	mUnk0x1C = 0;
	mUnk0x20 = false;
	mUnk0x21 = false;
}

// FUNCTION: POPCAPGAME1 0x0043e4e0
void DebugMgr::SetMouseBall(Ball* param_1)
{
	if (mMouseBall != NULL)
		mMouseBall->mUnk0x140 = false;
	mMouseBall = param_1;
	if (mMouseBall != NULL)
		mMouseBall->mUnk0x140 = true;
}

// FUNCTION: POPCAPGAME1 0x0043e520
bool DebugMgr::MouseUp(int theX, int theY, int theClickCount)
{
	if (mUnk0x4)
	{
		SetMouseBall(NULL);
		return true;
	}
	return false;
}

// FUNCTION: POPCAPGAME1 0x0044b4a0
bool DebugMgr::MouseDown(int theX, int theY, int theClickCount)
{
	theX -= mBoard->mUnk0x1a8;
	theY -= mBoard->mUnk0x1ac;
	LogicMgr* logicMgr = mBoard->mLogicMgr;
	bool currentPlayerFlag = *(&logicMgr->mUnk0x244 + logicMgr->mUnk0x128);
	if ((theClickCount >= 0 || logicMgr->mUnk0x4 != 1 || currentPlayerFlag) &&
		theClickCount > 0 && mUnk0x4) {
		SetMouseBall(GetBallAt((float)theX, (float)theY));
		if (mMouseBall != NULL) {
			mMouseOffsetX = mMouseBall->mUnk0xec - (float)theX;
			mMouseOffsetY = mMouseBall->mUnk0xf0 - (float)theY;
		}
		return true;
	}
	return false;
}

// FUNCTION: POPCAPGAME1 0x0044db10
void DebugMgr::DeleteBalls(bool param_1)
{
	std::list<SmartPtr<Ball> > balls;
	Ball* ballToKeep = NULL;
	float topY = 100000.0f;

	for (std::list<SmartPtr<Ball> >::iterator it = mBoard->mUnk0x19c.begin();
		 it != mBoard->mUnk0x19c.end(); ++it) {
		Ball* ball = *it;
		if (!ball->mUnk0x140) {
			if (ball->mUnk0xf0 < topY) {
				topY = ball->mUnk0xf0;
				ballToKeep = ball;
			}
			balls.push_back(ball);
		}
	}

	if (balls.size() < 2 || param_1) {
		ballToKeep = NULL;
	}
	for (std::list<SmartPtr<Ball> >::iterator it = balls.begin(); it != balls.end(); ++it) {
		if (it->get() != ballToKeep) {
			mBoard->RemoveObj(it->get());
		}
	}
}

// FUNCTION: POPCAPGAME1 0x0044dcd0
void DebugMgr::Update()
{
	if (mBoard->mUnk0xc2) {
		return;
	}
	if (mUnk0x1C != 0) {
		--mUnk0x1C;
		if (mUnk0x1C == 5) {
			DeleteBalls(true);
			UpdateMouseBall();
			return;
		}
		if (mUnk0x1C == 3 || mUnk0x1C == 1) {
			mBoard->mLogicMgr->mUnk0x8 = 100;
		}
	}
	UpdateMouseBall();
}
