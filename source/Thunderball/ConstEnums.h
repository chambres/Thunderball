#ifndef __CONSTENUMS_H__
#define __CONSTENUMS_H__

namespace Sexy
{
enum GameMode {
	UNKNOWN_0 = 0,
	ADVENTURE = 1,
	QUICK_PLAY = 2,
	DUEL = 3,
	CHALLENGE = 4,
	UNKNOWN_5 = 5,
	DEMO = 6
};

enum PowerupType {
	POWERUP_0 = 0,
	POWERUP_1 = 1,
	POWERUP_2 = 2,
	POWERUP_3 = 3,
	POWERUP_4 = 4,
	POWERUP_5 = 5,
	POWERUP_6 = 6,
	POWERUP_7 = 7,
	POWERUP_8 = 8,
	POWERUP_9 = 9,
	POWERUP_10 = 10,
	POWERUP_11 = 11,
	POWERUP_12 = 12,
	POWERUP_13 = 13
};

enum StyleShot;
enum LogicState;

enum PegType {
	NONE = 0,
	NORMAL = 1,
	GOAL = 2,
	SCORE = 3,
	POWERUP = 4
};

enum EndLevelMode;


} // namespace Sexy

#endif // __CONSTENUMS_H__
