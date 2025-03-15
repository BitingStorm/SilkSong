#include "PlayerAnimator.h"
#include "Player.h"
#include "GameplayStatics.h"
#include "Components/AudioPlayer.h"



PlayerAnimator::PlayerAnimator()
{
	idle.Load("player_idle");
	idle.SetInterval(0.1f);
	walk.Load("player_walk");
	walk.SetInterval(0.1f);
	walkstart.Load("player_walkstart");
	walkstart.SetInterval(0.04f);
	walkend.Load("player_walkend");
	walkend.SetInterval(0.06f);
	rush.Load("player_rush", { 0,15 });
	rush.SetInterval(0.08f);
	turn.Load("player_turn");
	turn.SetInterval(0.08f);
	jump.Load("player_jump");
	jump.SetInterval(0.08f);
	rushjump.Load("player_rushjump");
	rushjump.SetInterval(0.04f);
	fall.Load("player_fall");
	fall.SetInterval(0.1f);
	hardland.Load("player_hardland");
	hardland.SetInterval(0.15f);
	softland.Load("player_softland");
	softland.SetInterval(0.12f);
	attack_0.Load("player_attack_0", { 0,10 });
	attack_0.SetInterval(0.06f);
	attack_1.Load("player_attack_1", { 0,10 });
	attack_1.SetInterval(0.06f);
	attackup.Load("player_attackup");
	attackup.SetInterval(0.06f);
	attackdown.Load("player_attackdown");
	attackdown.SetInterval(0.06f);
	attackbounce.Load("player_attackbounce");
	attackbounce.SetInterval(0.05f);
	evade.Load("player_evade");
	evade.SetInterval(0.08f);
	dash.Load("player_dash", { 0,10 });
	dash.SetInterval(0.06f);
	airdash.Load("player_airdash");
	airdash.SetInterval(0.06f);
	cure.Load("player_cure");
	cure.SetInterval(0.04f);
	hurt.Load("player_hurt");
	hurt.SetInterval(0.08f);
	throw_.Load("player_throw");
	throw_.SetInterval(0.06f);
	grab.Load("player_grab");
	grab.SetInterval(0.07f);
	_closeskill.Load("player__closeskill");
	_closeskill.SetInterval(0.06f);
	closeskill.Load("player_closeskill");
	closeskill.SetInterval(0.06f);
	remoteskill.Load("player_remoteskill", {0,5});
    remoteskill.SetInterval(0.08f);
	die.Load("player_die");
	die.SetInterval(0.08f);
	lowhealth.Load("player_lowhealth", { 0,5 });
	lowhealth.SetInterval(0.1f);
	sitdown.Load("player_sitdown", {0,-5});
	sitdown.SetInterval(0.1f);
	standup.Load("player_standup");
	standup.SetInterval(0.15f);
	leave.Load("player_leave");
	leave.SetInterval(0.08f);
	wall.Load("player_wall", {20,0});
	wall.SetInterval(0.1f);
	lookdown.Load("player_lookdown", { -3,0 });
	lookdown.SetInterval(0.1f);
	lookup.Load("player_lookup", { -15,20 });
	lookup.SetInterval(0.1f);
	defend.Load("player_defend", { 0,20 });
	defend.SetInterval(0.1f);
	defendstart.Load("player_defendstart");
	defendstart.SetInterval(0.1f);
	defendend.Load("player_defendend");
	defendend.SetInterval(0.1f);
	defendattack.Load("player_defendattack");
	defendattack.SetInterval(0.1f);
	scare.Load("player_scare");
	scare.SetInterval(0.1f);

	Insert("idle", idle);
	Insert("walk", walk);
	Insert("walkstart", walkstart);
	Insert("walkend", walkend);
	Insert("rush", rush);
	Insert("turn", turn);
	Insert("jump", jump);
	Insert("rushjump", rushjump);
    Insert("fall", fall);
	Insert("hardland", hardland);
	Insert("attack_0", attack_0);
	Insert("attack_1", attack_1);
	Insert("attackup", attackup);
	Insert("attackdown", attackdown);
	Insert("attackbounce", attackbounce);
	Insert("softland", softland);
	Insert("evade", evade);
	Insert("dash", dash);
	Insert("airdash", airdash);
	Insert("cure", cure);
	Insert("hurt", hurt);
	Insert("throw", throw_); 
	Insert("grab", grab);
	Insert("_closeskill", _closeskill);
	Insert("closeskill", closeskill);
	Insert("remoteskill", remoteskill);
	Insert("die", die);
	Insert("lowhealth", lowhealth);
	Insert("sitdown", sitdown);
	Insert("standup", standup);
	Insert("lookdown", lookdown);
	Insert("lookup", lookup);
	Insert("leave", leave);
	Insert("wall", wall);
	Insert("defend", defend);
	Insert("defendstart", defendstart);
	Insert("defendend", defendend);
	Insert("defendattack", defendattack);
	Insert("scare", scare);
	SetNode("idle");


	AddParamater("flying", ParamType::Bool);
	AddParamater("walkingSpeed", ParamType::Float);
	AddParamater("landingSpeed", ParamType::Float);
	AddParamater("fallingSpeed", ParamType::Float);
	AddParamater("floatingEnd", ParamType::Trigger);
	AddParamater("validDownAttack", ParamType::Bool);
	AddParamater("leaveWall", ParamType::Trigger);
	AddParamater("lookFlag", ParamType::Integer);
	AddParamater("lowHealth", ParamType::Bool);
}

void PlayerAnimator::BeginPlay()
{
	Animator::BeginPlay();

	idle_to_walkstart.Init(idle, walkstart);
	idle_to_walkstart.AddCondition(AnimTransition::Float{ "walkingSpeed",10.f,TransitionComparison::GreaterEqual });
	idle_to_walkstart.AddCondition(AnimTransition::Bool{ "flying", false });
	
	walk_to_rush.Init(walk, rush);
	walk_to_rush.AddCondition(AnimTransition::Float{ "walkingSpeed",500.f,TransitionComparison::GreaterEqual });
	rush_to_walk.Init(rush, walk);
	rush_to_walk.AddCondition(AnimTransition::Float{ "walkingSpeed",500.f,TransitionComparison::Less });
	walk_to_walkend.Init(walk, walkend);
	walk_to_walkend.AddCondition(AnimTransition::Float{ "walkingSpeed",10.f,TransitionComparison::Less });
	walkstart_to_walk.Init(walkstart, walk);
	walkend_to_idle.Init(walkend, idle);

	hardland_to_idle.Init(hardland, idle);
	softland_to_idle.Init(softland, idle);
	attack_0_to_idle.Init(attack_0, idle);
	attack_1_to_idle.Init(attack_1, idle);
	attackup_to_idle.Init(attackup, idle);
	attackdown_to_idle.Init(attackdown, idle);
	attackdown_to_idle.AddCondition(AnimTransition::Bool{ "validDownAttack",false });
	attackdown_to_attackbounce.Init(attackdown, attackbounce);
	attackdown_to_attackbounce.AddCondition(AnimTransition::Bool{ "validDownAttack",true });
	attackbounce_to_idle.Init(attackbounce, idle);
	jump_to_fall.Init(jump, fall);
	rushjump_to_fall.Init(rushjump, fall);
	evade_to_idle.Init(evade, idle);
	cure_to_idle.Init(cure, idle);
	hurt_to_idle.Init(hurt, idle);
	dash_to_idle.Init(dash, idle);
	airdash_to_fall.Init(airdash, fall);
	throw_to_idle.Init(throw_, idle);
	grab_to_idle.Init(grab, idle);
	remoteskill_to_idle.Init(remoteskill, idle);
	_closeskill_to_closeskill.Init(_closeskill, closeskill);
	closeskill_to_idle.Init(closeskill, idle);
	closeskill_to_idle.AddCondition(AnimTransition::Trigger{ "floatingEnd" });
	standup_to_idle.Init(standup, idle);
	lookdown_to_idle.Init(lookdown, idle);
	lookdown_to_idle.AddCondition(AnimTransition::Integer{ "lookFlag",0,TransitionComparison::Equal });
	idle_to_lookdown.Init(idle, lookdown);
	idle_to_lookdown.AddCondition(AnimTransition::Integer{ "lookFlag",1,TransitionComparison::Equal });
	standup_to_idle.Init(standup, idle);
	lookup_to_idle.Init(lookup, idle);
	lookup_to_idle.AddCondition(AnimTransition::Integer{ "lookFlag",0,TransitionComparison::Equal });
	idle_to_lookup.Init(idle, lookup);
	idle_to_lookup.AddCondition(AnimTransition::Integer{ "lookFlag",2,TransitionComparison::Equal });
	wall_to_idle.Init(wall, idle);
	wall_to_idle.AddCondition(AnimTransition::Trigger{ "leaveWall" });

	idle_to_fall.Init(idle, fall);
	idle_to_fall.AddCondition(AnimTransition::Bool{ "flying",true });
	idle_to_fall.AddCondition(AnimTransition::Float{ "fallingSpeed",10.f,TransitionComparison::Greater });

	walk_to_fall.Init(walk, fall);
	walk_to_fall.AddCondition(AnimTransition::Bool{ "flying",true });

	fall_to_hardland.Init(fall, hardland);
	fall_to_hardland.AddCondition(AnimTransition::Float{ "landingSpeed",1200.f,TransitionComparison::Greater });

	fall_to_softland.Init(fall, softland);
	fall_to_softland.AddCondition(AnimTransition::Float{ "landingSpeed",1200.f,TransitionComparison::LessEqual });
	fall_to_softland.AddCondition(AnimTransition::Float{ "landingSpeed",500.f,TransitionComparison::Greater });
	fall_to_softland.AddCondition(AnimTransition::Float{ "walkingSpeed",100.f,TransitionComparison::Less });

	fall_to_idle.Init(fall, idle);
	fall_to_idle.AddCondition(AnimTransition::Float{ "fallingSpeed",0.f,TransitionComparison::LessEqual });
	fall_to_idle.AddCondition(AnimTransition::Bool{ "flying",false });

	leave_to_fall.Init(leave, fall);
	defendstart_to_defend.Init(defendstart, defend);
	defend_to_defendend.Init(defend, defendend);
	defendend_to_idle.Init(defendend, idle);
	defendattack_to_idle.Init(defendattack, idle);
	
	idle_to_lowhealth.Init(idle, lowhealth);
	idle_to_lowhealth.AddCondition(AnimTransition::Bool{ "lowHealth",true });
	lowhealth_to_idle.Init(lowhealth, idle);
	lowhealth_to_idle.AddCondition(AnimTransition::Bool{ "lowHealth",false });


	if (Player* player = Cast<Player>(pOwner))
	{
		hardland.OnAnimEnter.Bind([=]() {player->EnableInput(false); GameplayStatics::PlayCameraShake(3, 5);});
		hardland.OnAnimExit.Bind([=]() {player->EnableInput(true); });
		evade.OnAnimEnter.Bind([=]() {player->EnableInput(false); });
		evade.OnAnimExit.Bind([=]() {player->EnableInput(true); });
		dash.OnAnimEnter.Bind([=]() {player->EnableInput(false); });
		dash.OnAnimExit.Bind([=]() {player->EnableInput(true); });
		dash.AddNotification(2, dashEffect);
		airdash.OnAnimEnter.Bind([=]() {player->EnableInput(false); });
		airdash.OnAnimExit.Bind([=]() {player->EnableInput(true); });
		airdash.AddNotification(1, dashEffect);
		cure.OnAnimEnter.Bind([=]() {player->EnableInput(false); });
		cure.OnAnimExit.Bind([=]() {player->SetFloating(false); });
		cure.AddNotification(5, cureEffect);
		hurt.OnAnimEnter.Bind([=]() {player->EnableInput(false); });
		hurt.OnAnimExit.Bind([=]() {player->EnableInput(true); });
		hurtPause.Bind([]() {GameplayStatics::Pause(0.25f); GameplayStatics::PlayCameraShake(7, 5); });
		hurt.AddNotification(1, hurtPause);
		walk.OnAnimEnter.Bind([=]() {
			AudioPlayer* audio = player->GetComponentByClass<AudioPlayer>();
			if (!audio)return;
			if (player->GetWorldPosition().y > 1000)audio->Play("sound_swim", true);
			else audio->Play("sound_waterwalk", true);
			});
		walk.AddNotification(2, wetWalkEffect);
		walk.AddNotification(6, wetWalkEffect);
		closeskill.OnAnimExit.Bind([=]() {player->SetFloating(false); });
		throw_.AddNotification(3, dartSpawn);
		grab.AddNotification(4, grabFinished);
		grab.OnAnimExit.Bind([=]() {player->EnableInput(true); });
		remoteskill.AddNotification(8, needleSpawn);
		remoteskill.OnAnimExit.Bind([=]() {player->EnableInput(true); });
		attackdown.AddNotification(2, downAttackSpawn);
		standup.OnAnimExit.Bind([=]() {player->StandUp(); });
		dieShake.Bind([]() {GameplayStatics::PlayCameraShake(5, 2); });
		die.AddNotification(1, dieShake);
		leave.OnAnimExit.Bind([=]() {player->LeaveUp(); });
		leave.AddNotification(3, leaveStart);
		wall.OnAnimExit.Bind([=]() {player->LeaveWall(); });
		/*defendstart.OnAnimEnter.Bind([=]() {player->Defend(true); });
		defendstart.OnAnimExit.Bind([=]() {player->Defend(false); });*/
		defend.OnAnimEnter.Bind([=]() {player->Defend(true); });
		defend.OnAnimExit.Bind([=]() {player->Defend(false); });
		defendPause.Bind([=]() {GameplayStatics::Pause(0.25f); GameplayStatics::PlayCameraShake(7, 5); player->AddSilk(3); });
		defendattack.AddNotification(1, defendPause);
		defendattack.AddNotification(3, defendAttack);
	}
}