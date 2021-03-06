#include "discord.h"
#include <ctime>
#include <sstream>
#include <iostream>


void logProblem(discord::LogLevel level, const char* message) {
	std::cout << "Discord:" << (int)level << " - " << message << std::endl;
}

void check(discord::Result result) {
	std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
		<< " updating activity!\n";
}

uint64_t Discord::CurrentTimeInSeconds(){
	return std::time(0);
}

void Discord::SetActivity(activityData data) {
	discord::Activity activity = data.ToActivity();
	discord::Core::Create(std::strtoll(data.id, nullptr, 10), DiscordCreateFlags_Default, &core);
	core->ActivityManager().UpdateActivity(activity, &check);
}

Discord::Discord(activityData data) {
	SetActivity(data);
}

void Discord::UpdateActivity(activityData data) {
	discord::Activity activity = data.ToActivity();
	core->ActivityManager().ClearActivity(&check);
	core->ActivityManager().UpdateActivity(activity, &check);
}

void Discord::RunCallbacks()
{
	core->RunCallbacks();
}

void Discord::ClearActivity(){
	core->ActivityManager().ClearActivity(&check);
}

discord::Activity Discord::activityData::ToActivity()
{
	discord::Activity activity{};
	activity.SetType(this->type);
	activity.SetState(this->state);
	activity.SetDetails(this->details);
	std::int64_t test = std::strtoll(this->endtime, nullptr, 10);
	if (std::strtoll(this->starttime, nullptr, 10) == 0 && std::strtoll(this->endtime, nullptr, 10) != 0) {
		activity.GetTimestamps().SetEnd(std::strtoll(this->endtime, nullptr, 10));
	}
	else if (std::strtoll(this->endtime, nullptr, 10) == 0 && std::strtoll(this->starttime, nullptr, 10) != 0) {
		activity.GetTimestamps().SetStart(std::strtoll(this->starttime, nullptr, 10));
	}
	else {
		activity.GetTimestamps().SetStart(CurrentTimeInSeconds());
	}
	activity.GetAssets().SetLargeImage(this->large_image);
	activity.GetAssets().SetLargeText(this->large_text);
	activity.GetAssets().SetSmallImage(this->small_image);
	activity.GetAssets().SetSmallText(this->small_text);
	return activity;
}

//Discord::activityData data{ discord::ActivityType::Playing , "965996061711822888" };
//Discord dis = Discord(data);
