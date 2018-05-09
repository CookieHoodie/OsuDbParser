#include "OsuDbParser.h"
#include <iostream>

using namespace std;

int main() {
	string fullPathToOsuDb = "C:\\Users\\ong\\AppData\\Local\\osu!\\osu!.db";
	OsuDbParser osuDb = OsuDbParser(fullPathToOsuDb);
	if (osuDb.beatmaps.size() == osuDb.numberOfBeatmaps) {
		cout << "Number of beatmaps: " << osuDb.numberOfBeatmaps << endl;
		cout << "Beatmap Kano - Himekoi (Fycho) [Insane].osu has " << osuDb.beatmaps.at("Kano - Himekoi (Fycho) [Insane].osu").numOfHitcircles << " HitCircles" << endl;
	}
	else {
		cerr << "Error parsing all beatmap. Consider changing the key of beatmaps dict." << endl;
	}
}