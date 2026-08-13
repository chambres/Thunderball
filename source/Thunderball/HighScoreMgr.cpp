#include "HighScoreMgr.h"

#include "DataSync.h"

#include <SexyAppFramework/Common.h>

#include <cstdlib>

using namespace Sexy;

namespace
{
void SyncHighScores(DataSync& theSync, HighScoreMgr::HighScoreMap& theHighScores)
{
	if (theSync.mReader != NULL) {
		theHighScores.clear();
		ulong aCount = theSync.mReader->ReadLong();
		for (ulong i = 0; i < aCount; ++i) {
			std::string aScoreName;
			bool anOldStringMode = theSync.mUnk0x4c;
			theSync.mUnk0x4c = false;
			theSync.SyncString(aScoreName);
			theSync.mUnk0x4c = anOldStringMode;

			DataSync_SyncSTLContainer(theSync, theHighScores[aScoreName]);
		}
	}
	else {
		theSync.mWriter->WriteLong((ulong) theHighScores.size());
		for (HighScoreMgr::HighScoreMap::iterator anItr = theHighScores.begin();
			 anItr != theHighScores.end(); ++anItr) {
			bool anOldStringMode = theSync.mUnk0x4c;
			theSync.mUnk0x4c = false;
			theSync.SyncString(const_cast<std::string&>(anItr->first));
			theSync.mUnk0x4c = anOldStringMode;

			DataSync_SyncSTLContainer(theSync, anItr->second);
		}
	}
}
}

// FUNCTION: POPCAPGAME1 0x00473e00
HighScoreMgr::HighScoreMgr()
{
}

// SYNTHETIC: POPCAPGAME1 0x00473e40
// Sexy::HighScoreMgr::`scalar deleting destructor'

// FUNCTION: POPCAPGAME1 0x00471e70
HighScoreMgr::~HighScoreMgr()
{
}

// FUNCTION: POPCAPGAME1 0x00436670
void HighScoreEntry::SyncState(DataSync& theSync)
{
	theSync.SyncString(mName);
	theSync.SyncLong(mScore);
}

// FUNCTION: POPCAPGAME1 0x0045c300
void HighScoreMgr::CreateDefHighScores(std::list<HighScoreEntry>* theList, std::string* param)
{
	const char* aNames[] = {
		"Andy", "Anthony", "Ben", "Bill", "Brian", "Ace", "Chad", "Dave",
		"Del", "Ed", "Jimmy Goggle", "Eric", "Hans", "Isaac", "Jason", "James",
		"Jeff", "Joe", "John", "Josh", "Juho", "Kathy", "Katrina", "Mark",
		"Matt", "Max", "Shawn", "Stephen", "Sukhbir", "Tysen", "Walter", "Wes"
	};

	bool isAdventure = param->compare(0, param->length(), "_adventure", 10) == 0;
	int aScore = isAdventure ? 20000000 : 300000;

	for (int i = 0; i < 10; ++i) {
		HighScoreEntry anEntry;
		anEntry.mName = aNames[rand() % 32];
		anEntry.mScore = aScore;
		theList->push_back(anEntry);

		if (isAdventure) {
			aScore -= i == 0 ? 3000000 : 1500000;
		}
		else {
			aScore -= 10000;
		}
	}
}

// FUNCTION: POPCAPGAME1 0x00471eb0
void HighScoreMgr::SyncState(DataSync& theSync)
{
	int aVersion = 2;
	theSync.SyncLong(aVersion);
	theSync.mVersion = aVersion;

	if (aVersion > 1) {
		SyncHighScores(theSync, mHighScores);
	}
}

// FUNCTION: POPCAPGAME1 0x00472060
bool HighScoreMgr::Save()
{
	MkDir(GetAppDataFolder() + "userdata");

	DataWriter aWriter;
	if (!aWriter.OpenFile(GetAppDataFolder() + "userdata/highscores.dat")) {
		return false;
	}

	DataSync aSync(aWriter);
	SyncState(aSync);
	aSync.SyncPointers();
	return true;
}

// FUNCTION: POPCAPGAME1 0x00471f00
bool HighScoreMgr::Load()
{
	mHighScores.clear();

	DataReader aReader;
	if (!aReader.OpenFile(GetAppDataFolder() + "userdata/highscores.dat")) {
		return false;
	}

	DataSync aSync(aReader);
	SyncState(aSync);
	aSync.SyncPointers();
	return true;
}

// FUNCTION: POPCAPGAME1 0x00472430
std::list<HighScoreEntry>* HighScoreMgr::GetScores(std::string* param_1, bool param_2)
{
	if (param_1->empty()) {
		return NULL;
	}

	std::list<HighScoreEntry>& aScores = mHighScores[*param_1];
	if (aScores.empty()) {
		CreateDefHighScores(&aScores, param_1);
	}

	if (param_2) {
		std::list<HighScoreEntry>::iterator it = aScores.begin();
		while (it != aScores.end() && !it->mIsActive) {
			++it;
		}
		if (it == aScores.end()) {
			return NULL;
		}
	}

	return &aScores;
}

// FUNCTION: POPCAPGAME1 0x00472210
bool HighScoreMgr::Submit(std::string* theScoreName, std::string* thePlayerName, int theScore,
	bool theIsNewHighScore, bool theReload)
{
	if (theScore <= 0) {
		return false;
	}

	std::list<HighScoreEntry>& aScores = mHighScores[*theScoreName];
	if (aScores.empty()) {
		CreateDefHighScores(&aScores, theScoreName);
	}

	for (std::list<HighScoreEntry>::iterator anItr = aScores.begin();
		 anItr != aScores.end(); ++anItr) {
		anItr->mIsActive = false;
	}

	std::list<HighScoreEntry>::iterator anInsertItr = aScores.begin();
	while (anInsertItr != aScores.end() && anInsertItr->mScore >= theScore) {
		++anInsertItr;
	}

	if (anInsertItr == aScores.end() && aScores.size() >= 10) {
		return false;
	}

	if (theIsNewHighScore) {
		if (theReload) {
			Load();
			return Submit(theScoreName, thePlayerName, theScore, true, false);
		}

		HighScoreEntry anEntry;
		anEntry.mName = *thePlayerName;
		anEntry.mScore = theScore;
		anEntry.mIsActive = true;
		aScores.insert(anInsertItr, anEntry);

		while (aScores.size() > 10) {
			aScores.pop_back();
		}
		Save();
	}

	return true;
}
