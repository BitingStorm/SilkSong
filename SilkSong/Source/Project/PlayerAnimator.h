#pragma once
#include"Components/Animator.h"



class PlayerAnimator : public Animator
{
public:
	PlayerAnimator();

	void BeginPlay()override;

	AnimationDelegate wetWalkEffect;
	AnimationDelegate dashEffect;
	AnimationDelegate cureEffect;
	AnimationDelegate hurtPause;
	AnimationDelegate dartSpawn;
	AnimationDelegate needleSpawn;
	AnimationDelegate grabFinished;
	AnimationDelegate downAttackSpawn;
	AnimationDelegate dieShake;
	AnimationDelegate leaveStart;

private:
	Animation idle;
	Animation walk;
	Animation walkstart;
	Animation walkend;
	Animation rush;
	Animation turn;
	Animation jump;
	Animation rushjump;
	Animation fall;
	Animation hardland;
	Animation softland;
	Animation attack_0;
	Animation attack_1;
	Animation attackup;
	Animation attackdown;
	Animation attackbounce;
	Animation evade;
	Animation dash;
	Animation airdash;
	Animation cure;
	Animation hurt;
	Animation throw_;
	Animation grab;
	Animation _closeskill;
	Animation closeskill;
	Animation remoteskill;
	Animation die;
	Animation sitdown;
	Animation standup;
	Animation leave;

	AnimEdge idle_to_walkstart;
	AnimEdge walkstart_to_walk;
	AnimEdge walk_to_rush;
	AnimEdge rush_to_walk;
	AnimEdge walk_to_walkend;
	AnimEdge walkend_to_idle;
	AnimEdge jump_to_fall;
	AnimEdge rushjump_to_fall;
	AnimEdge hardland_to_idle;
	AnimEdge softland_to_idle;
	AnimEdge attack_0_to_idle;
	AnimEdge attack_1_to_idle;
	AnimEdge attackup_to_idle;
	AnimEdge attackdown_to_idle;
	AnimEdge attackdown_to_attackbounce;
	AnimEdge attackbounce_to_idle;
	AnimEdge idle_to_fall;
	AnimEdge walk_to_fall;
	AnimEdge fall_to_hardland;
	AnimEdge fall_to_softland;
	AnimEdge fall_to_idle;
	AnimEdge evade_to_idle;
	AnimEdge dash_to_idle;
	AnimEdge airdash_to_fall;
	AnimEdge cure_to_idle;
	AnimEdge hurt_to_idle;
	AnimEdge throw_to_idle;
	AnimEdge grab_to_idle;
	AnimEdge _closeskill_to_closeskill;
	AnimEdge closeskill_to_idle;
	AnimEdge remoteskill_to_idle;
	AnimEdge standup_to_idle;
	AnimEdge leave_to_fall;
};