#pragma once
struct Parameters {

	double player_hp = 3;

	double player_energyToShoot = 0.3;
	double player_energy = 10;
	double player_energy_recovery = 0;

	double player_staminaToShoot = 2.5;
	double player_stamina = 10;
	double player_stamina_recovery = 2;


	double player_damage = 1;
	double player_gun_cooldown = 0.5;

	double player_bullet_vel = 15;
	double player_bullet_force = 0;
	double player_bullet_lifetime = 10;
	double bullet_radius = 0.4;
	double bullet_mass = 0.01;



	//Misc
	double bonus_pickupFactor = 2; // Pickup_radius = Factor * your_radius
	double module_rocket_cooldown = 0.5;  //no idea what it does
	bool   isSuicideDecreaseKills = 0;
	double collision_regenCoeff = 0.2;  //regeneration of velocity in collisions
	double duration_Immframes = 0.1;
	double explosion_critMassRatio = 2;



	// Rocket param
    int rocket_binSearchPrecision = 20;
    double rocket_targetingRadius = 6.0;
    double rocket_triggerRadius = 0.6;

    bool rocket_explosion_isMove = 0;
    double rocket_explosion_radius = 2.0;
    double rocket_explosion_power = 5.0;
    double rocket_explosion_time = 0.1;

	double rocket_force = 15;
	double rocket_damage = 1;
	double rocket_initVel = -0.5;
	double rocket_radius = 0.4;

	// Force param
	double force_radius = 6;
	double force_power = 13;

	//mass
	double mass_duration = 5;
	double mass_mass = 0.2;
	double mass_radius = 0.5;
	double mass_vel = 18;
	double mass_cheatShove = 1.;
	double mass_cheatSlowdown = 4;
	double mass_effect = mass_duration / mass_mass; // Duration/mass

	//newbie immortality aka admin bonus #10
	char admin_name[10] = "ADMIN228";
	double admin_duration = 100000.0;



	//Varying bonus param
	double onKill_energyRegenBoost = 1.26;  
	double onKill_energyPlus = 5;

	double berserk_firingBoost = 3.0;
	double berserk_duration = 5.0;

	double boost_damage = 5;
	double boost_speedIncrease = 5.0;
	double boost_duration = 5.0;

	double bonus_laser_damage = 3;
	double laser_duration = 0.5;

	double hpUp_hpIncrease = 1;
	double energyUp_energyInrease = 5;

	double impulse_deltaVel = 15;
	double impulse_aftereffectBoost = 2;
	double impulse_aftereffetDuration = 1;

	double immortality_duration = 5.0;
	double bonusImmortality_duration = 1.0;

	double blink_distance = 5;
	
	double invisibility_duration = 5.0;
	
};

