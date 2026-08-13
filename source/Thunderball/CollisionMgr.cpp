#include "CollisionMgr.h"

#include "Ball.h"
#include "Hole.h"
#include "Mover.h"
#include "PhysObj.h"
#include "ThunderCommon.h"

using namespace Sexy;

// GLOBAL: POPCAPGAME1 0x00650a70
CollisionMgr* CollisionMgr::mUpdateMgr = NULL;

static std::pair<PhysObj*, PhysObj*> MakeCollisionPair(PhysObj* param_1, PhysObj* param_2)
{
	if (param_1->mUnk0x10 == 2 &&
		(param_2->mUnk0x10 != 2 || param_1->mCurSortId < param_2->mCurSortId)) {
		return std::make_pair(param_1, param_2);
	}
	return std::make_pair(param_2, param_1);
}

static bool CanTrackCollision(PhysObj* param_1, PhysObj* param_2)
{
	if (param_1->mUnk0x10 != 2 && param_2->mUnk0x10 != 2) {
		return false;
	}
	if (param_1->mUnk0x10 == 2 && param_2->mUnk0x10 == 2 &&
		(!static_cast<Ball*>(param_1)->mUnk0x163 || !static_cast<Ball*>(param_2)->mUnk0x163)) {
		return false;
	}
	PhysObj* ball = param_1->mUnk0x10 == 2 ? param_1 : param_2;
	PhysObj* other = ball == param_1 ? param_2 : param_1;
	return ball->IsCollidableWith(other);
}

// FUNCTION: POPCAPGAME1 0x0044d870
CollisionMgr::CollisionMgr() : mUnk0x30(0), mUnk0x34(0)
{
	mUnk0x0 = false;
	mUnk0x78 = false;
}

// FUNCTION: POPCAPGAME1 0x00423fa0
CollisionMgr::~CollisionMgr()
{
}

// FUNCTION: POPCAPGAME1 0x00443660
void CollisionMgr::RemoveObj(PhysObj* param_1, std::vector<EndPoint>* param_2)
{
	int anIndex = 0;
	while (anIndex < (int)param_2->size())
	{
		if ((*param_2)[anIndex].mObject == param_1)
			param_2->erase(param_2->begin() + anIndex);
		else
			++anIndex;
	}
}

// FUNCTION: POPCAPGAME1 0x004465a0
void CollisionMgr::RemoveObj(PhysObj* param_1)
{
	RemoveObj(param_1, &mObjects1);
	RemoveObj(param_1, &mObjects2);
	std::set<std::pair<PhysObj*, PhysObj*> >::iterator anItr = mCollisions.begin();
	while (anItr != mCollisions.end())
	{
		if (anItr->first == param_1 || anItr->second == param_1)
			mCollisions.erase(anItr++);
		else
			++anItr;
	}
}

// FUNCTION: POPCAPGAME1 0x0044b1e0
void CollisionMgr::SortEndPoints(std::vector<EndPoint>* param_1)
{
	for (size_t i = 1; i < param_1->size(); ++i) {
		size_t index = i;
		while (index > 0 && *(*param_1)[index].mValue <= *(*param_1)[index - 1].mValue) {
			EndPoint& previous = (*param_1)[index - 1];
			EndPoint& current = (*param_1)[index];
			if (!previous.mIsMin && current.mIsMin) {
				if (CanTrackCollision(previous.mObject, current.mObject)) {
					mCollisions.insert(MakeCollisionPair(previous.mObject, current.mObject));
				}
			} else if (previous.mIsMin && !current.mIsMin) {
				mCollisions.erase(MakeCollisionPair(previous.mObject, current.mObject));
			}
			std::swap(previous, current);
			--index;
		}
	}
}

// FUNCTION: POPCAPGAME1 0x00446560
void CollisionMgr::Clear()
{
	mCollisions.clear();
	mObjects1.clear();
	mObjects2.clear();
}

// FUNCTION: POPCAPGAME1 0x0044d920
void CollisionMgr::AddObj(PhysObj* param_1, float* param_2, float* param_3, std::vector<EndPoint>* param_4)
{
	EndPoint minPoint = {param_1, param_2, true};
	EndPoint maxPoint = {param_1, param_3, false};
	param_4->push_back(minPoint);
	param_4->push_back(maxPoint);
}

// FUNCTION: POPCAPGAME1 0x0044d9d0
void CollisionMgr::AddObj(PhysObj* param_1)
{
	for (std::vector<EndPoint>::iterator it = mObjects1.begin(); it != mObjects1.end(); ++it) {
		if (it->mIsMin && CanTrackCollision(param_1, it->mObject)) {
			mCollisions.insert(MakeCollisionPair(param_1, it->mObject));
		}
	}
	AddObj(param_1, &param_1->mUnk0x14, &param_1->mUnk0x1c, &mObjects1);
	AddObj(param_1, &param_1->mUnk0x18, &param_1->mUnk0x20, &mObjects2);
}

// FUNCTION: POPCAPGAME1 0x0045ad30
void CollisionMgr::NotifyCollision(PhysObj* param_1, PhysObj* param_2)
{
	if (mUpdateMgr != NULL && (param_1->mUnk0x10 == 2 || param_2->mUnk0x10 == 2)) {
		std::pair<PhysObj*, PhysObj*> collision = MakeCollisionPair(param_1, param_2);
		mUpdateMgr->mUnk0x48.push_back(std::make_pair(
			SmartPtr<PhysObj>(collision.first), SmartPtr<PhysObj>(collision.second)));
	}
}

// FUNCTION: POPCAPGAME1 0x0045add0
void CollisionMgr::Update(bool param_1)
{
	mUnk0x38.clear();
	if (param_1) {
		PhysObj::mNotifyCollisionFunc = NotifyCollision;
		mUpdateMgr = this;
		mUnk0x48.clear();
	}

	SortEndPoints(&mObjects1);
	SortEndPoints(&mObjects2);
	Ball* currentBall = NULL;
	for (std::set<std::pair<PhysObj*, PhysObj*> >::iterator it = mCollisions.begin();
		 it != mCollisions.end(); ++it) {
		Ball* ball = static_cast<Ball*>(it->first);
		PhysObj* obj = it->second;
		if (ball != currentBall) {
			if (currentBall != NULL) {
				currentBall->DoCollideUpdate(&mUnk0x38, NULL);
				mUnk0x38.clear();
			}
			currentBall = ball;
		}
		if (!ball->IsCollidableWith(obj)) {
			continue;
		}
		switch (obj->mUnk0x10) {
		case 1:
			static_cast<Hole*>(obj)->CheckCollision(ball);
			break;
		case 2:
			ball->CheckBallCollision(static_cast<Ball*>(obj));
			break;
		case 3:
		case 4:
		case 5:
			mUnk0x38.push_back(obj);
			break;
		}
	}
	if (currentBall != NULL) {
		currentBall->DoCollideUpdate(&mUnk0x38, NULL);
		mUnk0x38.clear();
	}
	PhysObj::mNotifyCollisionFunc = NULL;
}

// FUNCTION: POPCAPGAME1 0x00454dc0
void CollisionMgr::BeginUpdateOneBall(Ball* param_1)
{
	float margin = (float)ModVal(
		0, "SEXY_SEXYMODVALc:\\gamesrc\\cpp\\thunderball\\CollisionMgr.cpp81,266", 75);
	mUnk0x68.clear();
	mUnk0x58 = param_1->mUnk0x14 - margin;
	mUnk0x5C = param_1->mUnk0x1c + margin;
	mUnk0x60 = param_1->mUnk0x18 - margin;
	mUnk0x64 = param_1->mUnk0x20 + margin;

	for (std::vector<EndPoint>::iterator it = mObjects1.begin(); it != mObjects1.end(); ++it) {
		PhysObj* obj = it->mObject;
		if (!it->mIsMin && (mUnk0x78 || obj->mPegInfo == NULL) &&
			obj->mUnk0x14 <= mUnk0x5C && mUnk0x58 < obj->mUnk0x1c &&
			obj->mUnk0x18 <= mUnk0x64 && mUnk0x60 < obj->mUnk0x20) {
			mUnk0x68.push_back(obj);
		}
	}
}

// FUNCTION: POPCAPGAME1 0x00454f90
bool CollisionMgr::UpdateOneBall(Ball* param_1, SexyVector2* param_2)
{
	mUnk0x38.clear();
	bool didCollide = false;
	if (mUnk0x5C < param_1->mUnk0x1c || param_1->mUnk0x14 < mUnk0x58 ||
		param_1->mUnk0x18 < mUnk0x60 || mUnk0x64 < param_1->mUnk0x20) {
		BeginUpdateOneBall(param_1);
	}

	std::set<std::pair<PhysObj*, PhysObj*> > balls;
	std::set<std::pair<PhysObj*, PhysObj*> > objects;
	for (std::vector<PhysObj*>::iterator it = mUnk0x68.begin(); it != mUnk0x68.end(); ++it) {
		PhysObj* obj = *it;
		if (!obj->mUnk0x24 || obj->mUnk0x14 > param_1->mUnk0x1c ||
			param_1->mUnk0x14 >= obj->mUnk0x1c || obj->mUnk0x18 > param_1->mUnk0x20 ||
			param_1->mUnk0x18 >= obj->mUnk0x20) {
			continue;
		}

		switch (obj->mUnk0x10) {
		case 1: {
			Hole* hole = static_cast<Hole*>(obj);
			if (hole->mUnk0xe4 != NULL && hole->CheckCollision(param_1)) {
				mUnk0x0 = true;
				param_1->SetHoleOut(hole->mUnk0xe4, 0);
				if ((hole->mMover != NULL && hole->mUnk0x26) ||
					(hole->mUnk0xe4->mMover != NULL && hole->mUnk0xe4->mUnk0x26)) {
					didCollide = true;
					if (param_2 != NULL) {
						param_2->x = param_1->mUnk0xec;
						param_2->y = param_1->mUnk0xf0;
					}
				}
			}
			break;
		}
		case 3:
			balls.insert(MakeCollisionPair(param_1, obj));
			break;
		case 4:
		case 5:
			objects.insert(MakeCollisionPair(param_1, obj));
			break;
		}
	}

	for (std::set<std::pair<PhysObj*, PhysObj*> >::iterator it = balls.begin();
		 it != balls.end(); ++it) {
		if (param_1->CheckBallCollision(static_cast<Ball*>(it->second))) {
			didCollide = true;
		}
	}
	for (std::set<std::pair<PhysObj*, PhysObj*> >::iterator it = objects.begin();
		 it != objects.end(); ++it) {
		mUnk0x38.push_back(it->second);
	}
	if (!mUnk0x38.empty() && param_1->DoCollideUpdate(&mUnk0x38, param_2)) {
		didCollide = true;
	}
	if (!param_1->mUnk0x141) {
		param_1->DoNonCollideUpdate();
	}
	return didCollide;
}
