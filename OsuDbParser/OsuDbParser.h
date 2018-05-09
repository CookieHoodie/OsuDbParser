#pragma once

#include <string>
#include <fstream>
#include <unordered_map>

using namespace std;

typedef struct OsuDbTimingPointData
{
	double BPM;
	double offset;
	bool inherited;
} OsuDbTimingPointDataS;

typedef struct OsuDbBeatmapData // assume version >= 20140609
{
	unsigned int bytesOfBeatmapEntry;
	string artistName;
	string artistNameUnicode;
	string songTitle;
	string songTitleUnicode;
	string creatorName;
	string difficulty;
	string audioFileName;
	string MD5Hash;
	string nameOfOsuFile;
	unsigned char rankedStatus;
	unsigned int numOfHitcircles;
	unsigned int numOfSliders;
	unsigned int numOfSpinners;
	unsigned long long lastModificationTime;
	float AR;
	float CS;
	float HP;
	float OD;
	double sliderVelocity;
	unordered_map<unsigned int, double> standardModStarPair;
	unordered_map<unsigned int, double> taikoModStarPair;
	unordered_map<unsigned int, double> CTBModStarPair;
	unordered_map<unsigned int, double> maniaModStarPair;
	unsigned int drainTime; // in sec
	unsigned int totalTime; // in milli sec
	unsigned int previewTime;
	vector<OsuDbTimingPointDataS> timingPoints;
	unsigned int beatmapID;
	unsigned int beatmapSetID;
	unsigned int threadID;
	unsigned char gradeStandard;
	unsigned char gradeTaiko;
	unsigned char gradeCTB;
	unsigned char gradeMania;
	unsigned short localOffset;
	float stackLeniency;
	unsigned char gameplayMode;
	string songSource;
	string songTags;
	unsigned short onlineOffset;
	string fontUsed;
	bool isUnplayed;
	unsigned long long lastPlayedTime;
	bool isOsz2;
	string folderName;
	unsigned long long lastCheckedTime;
	bool ignoreBeatmapSound;
	bool ignoreBeatmapSkin;
	bool disableStoryboard;
	bool disableVideo;
	bool visualOverride;
	char maniaScrollSpeed;
} OsuDbBeatmapDataS;

class OsuDbParser
{
public:
	// constructor and destructor
	OsuDbParser(string fullPathToOsuDb);
	~OsuDbParser();

	// member variables
	unsigned int osuVersion;
	unsigned int folderCount;
	bool accountUnlocked;
	string playerName;
	unsigned int numberOfBeatmaps;
	unordered_map<string, OsuDbBeatmapDataS> beatmaps;

private:
	// general functions
	bool readBool(ifstream &osuDbHandle);
	unsigned char readChar(ifstream &osuDbHandle);
	unsigned short readShort(ifstream &osuDbHandle);
	unsigned int readInt(ifstream &osuDbHandle);
	unsigned long long readLong(ifstream &osuDbHandle); // long in osu! webpage == longlong in c++
	unsigned int readByteToInt(ifstream &osuDbHandle, int size);
	float readFloat(ifstream &osuDbHandle);
	double readDouble(ifstream &osuDbHandle);
	string readString(ifstream &osuDbHandle);

	void processData(string fullPathToOsuDb);
};

