/*
  Stockfish, a UCI chess playing engine derived from Glaurung 2.1
  Copyright (C) 2004-2008 Tord Romstad (Glaurung author)
  Copyright (C) 2008-2015 Marco Costalba, Joona Kiiski, Tord Romstad
  Copyright (C) 2015-2016 Marco Costalba, Joona Kiiski, Gary Linscott, Tord Romstad

  Stockfish is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Stockfish is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SEARCH_H
#define SEARCH_H

#include <stdatomic.h>

#include "misc.h"
#include "position.h"
#include "thread.h"
#include "types.h"

typedef Value CounterMoveStats[16][64];
typedef CounterMoveStats CounterMoveHistoryStats[16][64];

// RootMove struct is used for moves at the root of the tree. For each root
// move we store a score and a PV (really a refutation in the case of moves
// which fail low). Score is normally set at -VALUE_INFINITE for all non-pv
// moves.

struct RootMove {
  size_t pv_size;
  Value score;
  Value previousScore;
  Move pv[MAX_PLY];
};

typedef struct RootMove RootMove;

struct RootMoves {
  size_t size;
  RootMove move[MAX_MOVES];
};

typedef struct RootMoves RootMoves;

/// LimitsType struct stores information sent by GUI about available time to
/// search the current move, maximum depth/time, if we are in analysis mode or
/// if we have to ponder while it's our opponent's turn to move.

struct LimitsType {
  int time[2];
  int inc[2];
  int npmsec;
  int movestogo;
  int depth;
  int movetime;
  int mate;
  int infinite;
  int ponder;
  uint64_t nodes;
  TimePoint startTime;
  int num_searchmoves;
  Move searchmoves[MAX_MOVES];
};

typedef struct LimitsType LimitsType;

INLINE int use_time_management(LimitsType *l)
{
  return !(l->mate | l->movetime | l->depth | l->nodes | l->infinite);
}

// The SignalsType struct stores atomic flags updated during the search
// typically in an async fashion e.g. to stop the search by the GUI.

struct SignalsType {
  atomic_bool stop;
  atomic_bool stopOnPonderhit;
  int searching;
  LOCK_T lock;
};

typedef struct SignalsType SignalsType;

extern SignalsType Signals;
extern LimitsType Limits;
extern int TB_RootInTB;

void search_init();
void search_clear();
uint64_t perft(Pos *pos, Depth depth);

#endif

