#pragma once
struct Parameters {

	double player_hp = 3;
	double player_energy = 10;
	double player_stamina = 10;
	double player_stamina_recovery = 2;
	double player_cooldown = 0.5;
	double player_damage = 0.5;

	double bonus_laser_damage = 3;
	double module_rocket_cooldown = 0.5;


	//rocket param
    int rocket_binSearchPrecision = 20;
    double rocket_targetingRadius = 6.0;
    double rocket_triggerRadius = 0.6;

    bool rocket_explosion_isMove = 0;
    double rocket_explosion_radius = 2.0;
    double rocket_explosion_power = 5.0;
    double rocket_explosion_time = 0.1;

	double rocket_force = 15;
	double rocket_damage = 1;
	double rocket_initVel = 0;

	//force param
	double force_radius = 6;
	double force_power = 13;


	//misc
	double berserk_firingBoost = 3.0;
	double berserk_duration = 5.0;

	double boost_speedIncrease = 5.0;
	double boost_duration = 5.0;

	double immortality_duration = 5.0;
	double laser_duration = 0.5;

	double hpUp_hpIncrease = 1;
	double energyUp_energyInrease = 5;

	double impulse_deltaVel = 15;
	double impulse_aftereffectBoost = 2;
	double impulse_aftereffetDuration = 1;

	double pickupImmortality_duration = 1.0;
	double blink_distance = 5;
	double invisibility_duration = 5.0;
	
	double hook_duration = 5.0;
	double hook_force = 9;
	double hook_distance = 20;
	double hook_angle = 0.3;

	double mass_duration = 4;
	double mass_mass = 0.2;
	double mass_vel = 18;
	double mass_cheatSlowdown = 2;
	double mass_effect = mass_duration / mass_mass; // duration/mass

	double bonus_pickupFactor = 2; //pickup_radius = Factor * your_radius
    
};

