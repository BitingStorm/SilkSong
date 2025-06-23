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
	AnimationDelegate needleSpawn_;
	AnimationDelegate grabFinished;
	AnimationDelegate downAttackSpawn;
	AnimationDelegate dieShake;
	AnimationDelegate leaveStart;
	AnimationDelegate defendPause;
	AnimationDelegate defendAttack;

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
	Animation rushattack;
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
	Animation rapidskill;
	Animation die;
	Animation lowhealth;
	Animation sitdown;
	Animation standup;
	Animation lookdown;
	Animation lookup;
	Animation leave;
	Animation leavestart;
	Animation leaveend;
	Animation wall;
	Animation defendstart;
	Animation defend;
	Animation defendend;
	Animation defendattack;
	Animation scare;
	

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
	AnimEdge rushattack_to_rush;
	AnimEdge idle_to_fall;
	AnimEdge walk_to_fall;
	AnimEdge rush_to_fall;
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
	AnimEdge rapidskill_to_idle;
	AnimEdge standup_to_idle;
	AnimEdge leavestart_to_leave;
	AnimEdge leave_to_leaveend;
	AnimEdge leaveend_to_fall;
	AnimEdge wall_to_idle;
	AnimEdge idle_to_lookup;
	AnimEdge lookup_to_idle;
	AnimEdge idle_to_lookdown;
	AnimEdge lookdown_to_idle;
	AnimEdge defendstart_to_defend;
	AnimEdge defend_to_defendend;
	AnimEdge defendend_to_idle;
	AnimEdge defendattack_to_idle;
	AnimEdge idle_to_lowhealth;
	AnimEdge lowhealth_to_idle;
};