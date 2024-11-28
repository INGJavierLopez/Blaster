// Fill out your copyright notice in the Description page of Project Settings.


#include "EndRound.h"

#include "Animation/WidgetAnimation.h"

void UEndRound::PlayAnimTeamRoundScores(bool Blue)
{
	if (Blue)
	{
		if(BlueTeamScoreAnim)
		PlayAnimationForward(BlueTeamScoreAnim);

	}
	else
	{
		if(RedTeamScoreAnim)
		PlayAnimationForward(RedTeamScoreAnim);

	}
}
