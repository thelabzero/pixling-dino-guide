// Dumps the frames of the firmware's live screens that the design sheet does not have (the Wi-Fi setup screens), as
// text for gen_guide_anims.py. Built by that script against the firmware repo; not meant to be run by hand.
//   c++ -std=c++17 -I<fw>/firmware/src export_live_frames.cpp <fw>/firmware/src/{face/face,scenes/dino_clips,scenes/dino_live}.cpp
// Output: "CLIP key|name|frames", then per frame "FRAME ms|note" and the 128x64 picture as 2048 hex digits
// (row-major, most significant bit = leftmost pixel: the same packing the guide uses for the sheet's frames).
#include <stdio.h>
#include <string.h>
#include "scenes/dino_live.h"

static void dump(const char* key, const char* name, utility::Clip clip, int a) {
  static face::Grid g;
  utility::View v;
  v.clip = clip; v.a = a;
  strcpy(v.text, "10:24");
  strcpy(v.ssid, "DINO-1234");      // examples: the real network name and password are random on every setup
  strcpy(v.code, "K7M2QX9PH4");
  scenes::LiveClip lc;
  scenes::buildLive(v, lc);
  printf("CLIP %s|%s|%zu\n", key, name, lc.frames.size());
  for (const scenes::Frame& f : lc.frames) {
    scenes::renderFrame(f, g);
    printf("FRAME %u|%s\n", (unsigned)f.ms, f.note);
    for (int i = 0; i < 128 * 64 / 8; i++) {
      unsigned b = 0;
      for (int k = 0; k < 8; k++) if (g.d[i * 8 + k]) b |= 0x80u >> k;
      printf("%02x", b);
    }
    printf("\n");
  }
}

int main() {
  dump("wifi_menu", "Wi-Fi card", utility::Clip::Menu, 4);
  dump("wifi_pair", "Looking for Wi-Fi", utility::Clip::WifiPair, 0);
  dump("wifi_joined", "Phone found", utility::Clip::WifiPair, 1);
  dump("wifi_connecting", "Connecting", utility::Clip::WifiPair, 2);
  dump("wifi_done", "Connected", utility::Clip::WifiDone, 0);
  dump("wifi_fail", "Did not connect", utility::Clip::WifiFail, 0);
  return 0;
}
