// ============================================================
// State Pattern Notes
// ============================================================
// Intent:
// State is a behavioral design pattern that lets an object alter its behavior
// when its internal state changes, as if it changed its class.
//
// Core idea:
// 1) Context keeps a reference to current State object.
// 2) Context delegates state-specific behavior to State.
// 3) Concrete states can trigger transitions to other states.
//
// Why not only if/switch:
// If state-based branches spread across many methods, maintenance becomes hard.
// State pattern extracts each branch family into dedicated classes.
//
// Advantages:
// 1) Removes large conditional blocks from context.
// 2) Follows Single Responsibility for state-specific behavior.
// 3) Makes adding new states easier.
//
// Trade-offs:
// 1) Adds extra classes for small state machines.
// 2) Transition logic must be designed carefully to avoid confusion.
//
// Relation with Strategy:
// Both use composition and delegation.
// Strategy objects are usually independent;
// State objects usually know transitions and can switch each other.
//
// ============================================================
// 状态模式笔记
// ============================================================
// 意图：
// 状态模式是一种行为型设计模式，让对象在内部状态变化时改变行为，
// 看起来就像切换了所属类。
//
// 核心思想：
// 1）上下文（Context）持有当前状态对象。
// 2）上下文将状态相关行为委派给状态对象。
// 3）具体状态可触发状态迁移。
//
// 为什么不只用 if/switch：
// 当状态分支散落到多个方法时，条件判断会越来越臃肿，维护成本高。
// 状态模式把同一状态下的行为抽到独立类中。
//
// 优点：
// 1）减少上下文中的复杂条件分支。
// 2）状态相关逻辑职责清晰。
// 3）扩展新状态更方便。
//
// 缺点：
// 1）对简单状态机会引入较多类。
// 2）状态迁移设计不当会导致流程难理解。
//
// 与策略模式关系：
// 两者都基于组合与委派。
// 策略通常彼此独立；状态通常知道迁移关系并可互相切换。

#include <iostream>
#include <memory>
#include <string>

using namespace std;

/*
Scenario Explanation:

This example models an audio player with three states:
1) ReadyState   : player is idle but unlocked.
2) PlayingState : player is currently playing.
3) LockedState  : player UI is locked.

Same UI actions (lock/play/next/previous) produce different behavior depending
on current state.
*/

/*
场景说明：

本例模拟一个音频播放器，包含三个状态：
1）ReadyState   ：空闲且未锁定。
2）PlayingState ：正在播放。
3）LockedState  ：界面锁定。

同样的用户操作（lock/play/next/previous）在不同状态下会产生不同结果。
*/

class AudioPlayer;

class State
{
public:
  virtual ~State() = default;

  void setPlayer(AudioPlayer *playerRef)
  {
    player = playerRef;
  }

  virtual const char *name() const = 0;
  virtual void clickLock() = 0;
  virtual void clickPlay() = 0;
  virtual void clickNext(bool doubleClick) = 0;
  virtual void clickPrevious(bool doubleClick) = 0;

protected:
  AudioPlayer *player = nullptr;
};

class ReadyState;
class PlayingState;
class LockedState;

class AudioPlayer
{
public:
  AudioPlayer();

  void changeState(unique_ptr<State> nextState)
  {
    state = std::move(nextState);
    state->setPlayer(this);
    cout << "[Player] state -> " << state->name() << "\n";
  }

  void clickLock()
  {
    state->clickLock();
  }

  void clickPlay()
  {
    state->clickPlay();
  }

  void clickNext(bool doubleClick = false)
  {
    state->clickNext(doubleClick);
  }

  void clickPrevious(bool doubleClick = false)
  {
    state->clickPrevious(doubleClick);
  }

  void startPlayback()
  {
    if (playlistSize == 0)
    {
      cout << "[Player] playlist is empty\n";
      return;
    }
    isPlaying = true;
    cout << "[Player] start playing: " << currentSongName() << "\n";
  }

  void stopPlayback()
  {
    isPlaying = false;
    cout << "[Player] stop playback\n";
  }

  void nextSong()
  {
    if (playlistSize == 0)
    {
      return;
    }
    currentSong = (currentSong + 1) % playlistSize;
    cout << "[Player] next song -> " << currentSongName() << "\n";
  }

  void previousSong()
  {
    if (playlistSize == 0)
    {
      return;
    }
    currentSong = (currentSong - 1 + playlistSize) % playlistSize;
    cout << "[Player] previous song -> " << currentSongName() << "\n";
  }

  void fastForward(int seconds)
  {
    cout << "[Player] fast-forward " << seconds << "s\n";
  }

  void rewind(int seconds)
  {
    cout << "[Player] rewind " << seconds << "s\n";
  }

  bool playing() const
  {
    return isPlaying;
  }

private:
  string currentSongName() const
  {
    return "song#" + to_string(currentSong + 1);
  }

private:
  unique_ptr<State> state;
  bool isPlaying = false;
  int playlistSize = 3;
  int currentSong = 0;
};

class ReadyState : public State
{
public:
  const char *name() const override
  {
    return "ReadyState";
  }

  void clickLock() override;

  void clickPlay() override;

  void clickNext(bool) override
  {
    player->nextSong();
  }

  void clickPrevious(bool) override
  {
    player->previousSong();
  }
};

class PlayingState : public State
{
public:
  const char *name() const override
  {
    return "PlayingState";
  }

  void clickLock() override;

  void clickPlay() override
  {
    player->stopPlayback();
    player->changeState(make_unique<ReadyState>());
  }

  void clickNext(bool doubleClick) override
  {
    if (doubleClick)
    {
      player->nextSong();
    }
    else
    {
      player->fastForward(5);
    }
  }

  void clickPrevious(bool doubleClick) override
  {
    if (doubleClick)
    {
      player->previousSong();
    }
    else
    {
      player->rewind(5);
    }
  }
};

class LockedState : public State
{
public:
  const char *name() const override
  {
    return "LockedState";
  }

  void clickLock() override;

  void clickPlay() override
  {
    cout << "[LockedState] ignore play (UI is locked)\n";
  }

  void clickNext(bool) override
  {
    cout << "[LockedState] ignore next (UI is locked)\n";
  }

  void clickPrevious(bool) override
  {
    cout << "[LockedState] ignore previous (UI is locked)\n";
  }
};

AudioPlayer::AudioPlayer()
{
  changeState(make_unique<ReadyState>());
}

void ReadyState::clickLock()
{
  player->changeState(make_unique<LockedState>());
}

void ReadyState::clickPlay()
{
  player->startPlayback();
  player->changeState(make_unique<PlayingState>());
}

void PlayingState::clickLock()
{
  player->changeState(make_unique<LockedState>());
}

void LockedState::clickLock()
{
  if (player->playing())
  {
    player->changeState(make_unique<PlayingState>());
  }
  else
  {
    player->changeState(make_unique<ReadyState>());
  }
}

int main()
{
  AudioPlayer player;

  cout << "\n== Ready actions ==\n";
  player.clickNext();
  player.clickPlay();

  cout << "\n== Playing actions ==\n";
  player.clickNext(false);
  player.clickNext(true);
  player.clickPrevious(false);

  cout << "\n== Lock while playing ==\n";
  player.clickLock();
  player.clickPlay();
  player.clickNext(true);

  cout << "\n== Unlock and continue ==\n";
  player.clickLock();
  player.clickPlay();

  return 0;
}