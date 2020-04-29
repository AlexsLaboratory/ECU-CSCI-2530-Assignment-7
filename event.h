#ifndef EVENT_H
#define EVENT_H

struct Event
{
  double time;
  int sender;
  int receiver;

  Event(double t, int s, int r) {
    time = t;
    sender = s;
    receiver = r;
  }
};

#endif