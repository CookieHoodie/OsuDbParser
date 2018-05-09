#include "OsuDbParser.h"

OsuDbParser::OsuDbParser(string fullPathToOsuDb)
{
	(this)->processData(fullPathToOsuDb);
}

OsuDbParser::~OsuDbParser()
{
}

void OsuDbParser::processData(string fullPathToOsuDb) {
	ifstream osuDbHandle;
	osuDbHandle.open(fullPathToOsuDb, ios::binary | ios::in);
	if (osuDbHandle) {
		(this)->osuVersion = (this)->readInt(osuDbHandle);
		(this)->folderCount = (this)->readInt(osuDbHandle);
		(this)->accountUnlocked = (this)->readBool(osuDbHandle);
		// skip through weird dateUnlock field which has 8 bytes
		osuDbHandle.seekg(8, ios::cur);
		(this)->playerName = (this)->readString(osuDbHandle);
		(this)->numberOfBeatmaps = (this)->readInt(osuDbHandle);

		// start parsing beatmapdata
		for (int b = 0; b < (this)->numberOfBeatmaps; b++) {
			OsuDbBeatmapDataS beatmap;
			beatmap.bytesOfBeatmapEntry = (this)->readInt(osuDbHandle);
			beatmap.artistName = (this)->readString(osuDbHandle);
			beatmap.artistNameUnicode = (this)->readString(osuDbHandle);
			beatmap.songTitle = (this)->readString(osuDbHandle);
			beatmap.songTitleUnicode = (this)->readString(osuDbHandle);
			beatmap.creatorName = (this)->readString(osuDbHandle);
			beatmap.difficulty = (this)->readString(osuDbHandle);
			beatmap.audioFileName = (this)->readString(osuDbHandle);
			beatmap.MD5Hash = (this)->readString(osuDbHandle);
			beatmap.nameOfOsuFile = (this)->readString(osuDbHandle);
			beatmap.rankedStatus = (this)->readChar(osuDbHandle);
			beatmap.numOfHitcircles = (this)->readByteToInt(osuDbHandle, 2);
			beatmap.numOfSliders = (this)->readByteToInt(osuDbHandle, 2);
			beatmap.numOfSpinners = (this)->readByteToInt(osuDbHandle, 2);
			beatmap.lastModificationTime = (this)->readLong(osuDbHandle);
			beatmap.AR = (this)->readFloat(osuDbHandle);
			beatmap.CS = (this)->readFloat(osuDbHandle);
			beatmap.HP = (this)->readFloat(osuDbHandle);
			beatmap.OD = (this)->readFloat(osuDbHandle);
			beatmap.sliderVelocity = (this)->readDouble(osuDbHandle);

			// starting of Int-Double Pairs
			// loop for 4 times: standard, taiko, CTB, and mania
			for (int i = 0; i < 4; i++) {
				unsigned int numberOfPairs = (this)->readInt(osuDbHandle);
				for (int j = 0; j < numberOfPairs; j++) {
					unsigned int modCombination;
					double starRating;
					unsigned char intPresent = (this)->readChar(osuDbHandle);
					if (intPresent == 0x08) { // if present. No use for now as this is the only value, but may change in future
						modCombination = (this)->readInt(osuDbHandle);
					}
					unsigned char doublePresent = (this)->readChar(osuDbHandle);
					if (doublePresent == 0x0d) {
						starRating = (this)->readDouble(osuDbHandle);
					}

					// it is assumed that all int and double are present
					switch (i) {
					case 0: {
						beatmap.standardModStarPair.insert({ modCombination, starRating });
						break;
					}
					case 1: {
						beatmap.taikoModStarPair.insert({ modCombination, starRating });
						break;
					}
					case 2: {
						beatmap.CTBModStarPair.insert({ modCombination, starRating });
						break;
					}
					case 3: {
						beatmap.maniaModStarPair.insert({ modCombination, starRating });
						break;
					}
					}
				}
			}
			// ending of Int-Double pairs

			beatmap.drainTime = (this)->readInt(osuDbHandle);
			beatmap.totalTime = (this)->readInt(osuDbHandle);
			beatmap.previewTime = (this)->readInt(osuDbHandle);

			// starting of timing points
			unsigned int numberOfTimingPoints = (this)->readInt(osuDbHandle);
			for (int i = 0; i < numberOfTimingPoints; i++) {
				OsuDbTimingPointDataS timingPoint;
				timingPoint.BPM = (this)->readDouble(osuDbHandle);
				timingPoint.offset = (this)->readDouble(osuDbHandle);
				timingPoint.inherited = (this)->readBool(osuDbHandle);
				beatmap.timingPoints.push_back(timingPoint);
			}
			// ending of timing points

			beatmap.beatmapID = (this)->readInt(osuDbHandle);
			beatmap.beatmapSetID = (this)->readInt(osuDbHandle);
			beatmap.threadID = (this)->readInt(osuDbHandle);
			beatmap.gradeStandard = (this)->readChar(osuDbHandle);
			beatmap.gradeTaiko = (this)->readChar(osuDbHandle);
			beatmap.gradeCTB = (this)->readChar(osuDbHandle);
			beatmap.gradeMania = (this)->readChar(osuDbHandle);
			beatmap.localOffset = (this)->readShort(osuDbHandle);
			beatmap.stackLeniency = (this)->readFloat(osuDbHandle);
			beatmap.gameplayMode = (this)->readChar(osuDbHandle);
			beatmap.songSource = (this)->readString(osuDbHandle);
			beatmap.songTags = (this)->readString(osuDbHandle);
			beatmap.onlineOffset = (this)->readShort(osuDbHandle);
			beatmap.fontUsed = (this)->readString(osuDbHandle);
			beatmap.isUnplayed = (this)->readBool(osuDbHandle);
			beatmap.lastPlayedTime = (this)->readLong(osuDbHandle);
			beatmap.isOsz2 = (this)->readBool(osuDbHandle);
			beatmap.folderName = (this)->readString(osuDbHandle);
			beatmap.lastCheckedTime = (this)->readLong(osuDbHandle);
			beatmap.ignoreBeatmapSound = (this)->readBool(osuDbHandle);
			beatmap.ignoreBeatmapSkin = (this)->readBool(osuDbHandle);
			beatmap.disableStoryboard = (this)->readBool(osuDbHandle);
			beatmap.disableVideo = (this)->readBool(osuDbHandle);
			beatmap.visualOverride = (this)->readBool(osuDbHandle);
			// skip through unknown int (last modification time?)
			osuDbHandle.seekg(sizeof(int), ios::cur);
			beatmap.maniaScrollSpeed = (this)->readChar(osuDbHandle);
			// finish parsing beatmap data
			// storing into member dict
			(this)->beatmaps.insert({ beatmap.nameOfOsuFile, beatmap });
		}
		// end parsing beatmaps

		// skip last unknown int
	}
	osuDbHandle.close();
}

bool OsuDbParser::readBool(ifstream &osuDbHandle) {
	bool output;
	osuDbHandle.read((char*)&output, sizeof(bool));
	return output;
}

unsigned char OsuDbParser::readChar(ifstream &osuDbHandle) {
	unsigned char output;
	osuDbHandle.read((char*)&output, sizeof(char));
	return output;
}

unsigned short OsuDbParser::readShort(ifstream &osuDbHandle) {
	unsigned short output;
	osuDbHandle.read((char*)&output, sizeof(short));
	return output;
}

unsigned int OsuDbParser::readInt(ifstream &osuDbHandle) {
	unsigned int output;
	osuDbHandle.read((char*)&output, sizeof(int));
	return output;
}

unsigned long long OsuDbParser::readLong(ifstream &osuDbHandle) {
	unsigned long long output;
	osuDbHandle.read((char*)&output, sizeof(long long));
	return output;
}

unsigned int OsuDbParser::readByteToInt(ifstream &osuDbHandle, int size) {
	unsigned int output = 0; // initialize to prevent junk value
	osuDbHandle.read((char*)&output, size);
	return output;
}

float OsuDbParser::readFloat(ifstream &osuDbHandle) {
	float output;
	osuDbHandle.read((char*)&output, sizeof(float));
	return output;
}

double OsuDbParser::readDouble(ifstream &osuDbHandle) {
	double output;
	osuDbHandle.read((char*)&output, sizeof(double));
	return output;
}

string OsuDbParser::readString(ifstream &osuDbHandle) {
	string output = ""; // return empty string if string is not found
	unsigned char stringPresent = (this)->readChar(osuDbHandle);
	if (stringPresent == 0x0b) { // if string is present
		unsigned int byteLengthOfString = 0;
		int shift = 0;
		while (true) {
			unsigned char bufferByte = (this)->readChar(osuDbHandle);
			// & 127 gets last 7 bits
			byteLengthOfString |= (bufferByte & 127) << shift;
			// & 128 gets 1st bit
			// if 1st bit == 0, the decode process ends
			if (!(bufferByte & (unsigned int)128)) {
				break;
			}
			shift += 7;
		}
		char* tempOutput = new char[byteLengthOfString + 1];
		osuDbHandle.read(tempOutput, byteLengthOfString);
		tempOutput[byteLengthOfString] = '\0';
		output = string(tempOutput);
		delete[] tempOutput;  // When done, free memory pointed to by a
	}
	return output;
}