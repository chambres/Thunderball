#ifndef __HIGHSCORE_MGR_H__
#define __HIGHSCORE_MGR_H__

#include <SexyAppFramework/Common.h>

#include <list>
#include <map>
#include <string>

namespace Sexy
{
class DataSync;

class HighScoreEntry {
public:
	std::string mName;
	int mScore;
	bool mIsActive;

	HighScoreEntry()
		: mName(), mScore(0), mIsActive(false)
	{
	}

	void SyncState(DataSync& theSync);
};

class HighScoreMgr {
public:
	typedef std::map<std::string, std::list<HighScoreEntry>, StringLessNoCase> HighScoreMap;

	HighScoreMap mHighScores;

	HighScoreMgr();
	virtual ~HighScoreMgr();

	void CreateDefHighScores(std::list<HighScoreEntry>* theList, std::string* param_1);
	void SyncState(DataSync& theSync);
	bool Save();
	bool Load();
	std::list<HighScoreEntry>* GetScores(std::string* param_1, bool param_2);
	bool Submit(std::string* theScoreName, std::string* thePlayerName, int theScore,
		bool theIsNewHighScore, bool theReload);
};

} // namespace Sexy

#endif // __HIGHSCORE_MGR_H__
