#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <GoScene.h>

void sideMenu(GoScene &scene, int &trialCount, long int &totalMoves, float &avgOverSq, int &boardSizeOption, bool &inProgress)
{

	ImGui::SetNextWindowPos(
		ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f),
		ImGuiCond_Always,
		ImVec2(1.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 3, 0.0f));

	ImGui::Begin("Stats", nullptr);
	ImGui::Text("Board Size: %dx%d", scene.game.width, scene.game.height);
	ImGui::Text("trials: %d", trialCount);
	float avg_moves{0};
	if (trialCount != 0)
		avg_moves = float(totalMoves / trialCount);
	ImGui::Text("Average moves: %f", avg_moves);
	ImGui::Text("avg_over_sq (updates every 10 games)%f", avgOverSq);

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Controls");
	bool size_changed;
	if (ImGui::InputInt("Board Size", &boardSizeOption))
		size_changed = true;
	ImGui::Checkbox("Render Board", &scene.enableRender);
	if (ImGui::Button("Start"))
	{
		scene.restart(boardSizeOption, boardSizeOption);
	}
	if (ImGui::Button("Reset") || size_changed)
	{
		scene.reset();
		trialCount = 0;
		totalMoves = 0;
		avgOverSq = 0;
		scene.capture_plot.clear();
	}

	if (inProgress)
	{
		if (ImGui::Button(scene.paused ? "Resume" : "Pause"))
		{
			scene.togglePause();
		}
	}
	if (ImGui::Button(scene.autoReset ? "Turn Off Auto-Reset" : "Turn On Auto-Reset"))
	{
		scene.toggleAutoReset();
	}

	ImGui::End();
}
void drawDataPlots(int trialCount, std::map<int, float> &data)
{
	ImGui::SetNextWindowPos(
		ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y),
		ImGuiCond_Always,
		ImVec2(1.0f, 1.0f));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x / 3, 0.0f));
	ImGui::Begin("data");

	std::vector<float> dataVec;
	for (auto i = data.begin(); i != data.end(); i++)
	{
		dataVec.push_back(float(i->second / trialCount));
	}

	ImGui::PlotHistogram("Average captures on each turn", &dataVec.front(), dataVec.size(), 0, "Average captures on each turn", 0.0f, FLT_MAX, ImVec2(400.f, 400.f), sizeof(dataVec.front()));
	ImGui::End();
}

int main()
{

	sf::RenderWindow window(sf::VideoMode({1000, 800}), "AutoGo");

	ImGui::SFML::Init(window);
	std::vector<float> avgMoveData;

	int trialCount = 0;
	long int totalMoves = 0;
	int startw = 9;
	int starth = 9;
	int blackCaptures = 0;
	int whiteCaptures = 0;
	float avgOverSq = 0.0f;
	int boardSizeOption = startw;
	bool done = false;
	bool inProgress = false;

	GoScene scene = GoScene(window, startw, starth);
	scene.enableRender = true; // disable to speed up runs
	sf::Clock deltaClock;
	while (window.isOpen())
	{
		while (const auto event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window, *event);

			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		sideMenu(scene, trialCount, totalMoves, avgOverSq, boardSizeOption, inProgress);
		drawDataPlots(trialCount, scene.capture_plot);

		window.clear();

		if (!done && !scene.paused)
		{
			int capturesThisGame = scene.game.blackCaptures + scene.game.whiteCaptures;
			scene.update();
			int capturesThisTurn = (scene.game.blackCaptures + scene.game.whiteCaptures) - capturesThisGame;
			if (scene.capture_plot.count(scene.game.moveCount))
			{
				scene.capture_plot[scene.game.moveCount] += capturesThisTurn;
			}
			else
			{
				scene.capture_plot[scene.game.moveCount] = capturesThisTurn;
			}

			if (scene.finished)
			{
				int area = scene.game.width * scene.game.height;
				trialCount++;
				if (trialCount % 10 == 0)
				{
					avgOverSq = float((totalMoves / trialCount)) / area;
				}

				totalMoves += scene.game.moveCount;
				blackCaptures += scene.game.blackCaptures;
				whiteCaptures += scene.game.whiteCaptures;
				if (scene.autoReset)
					scene.reset();
			}
		}
		else if (scene.paused)
			scene.update();

		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}