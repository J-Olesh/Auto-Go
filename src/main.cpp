#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <GoScene.h>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode({1000, 800}), "ImGui + SFML = <3");
	// window.setFramerateLimit(60);

	ImGui::SFML::Init(window);
	std::vector<float> avgMoveData;
	const int NUM_BOARD_SIZES = 20;
	const int TRIALS_PER_SIZE = 10;
	int sizesDone = 0;
	int trialCount = 0;
	int totalMoves = 0;
	int startw = 9;
	int starth = 9;
	bool done = false;
	GoScene scene = GoScene(window, startw, starth);

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

		ImGui::ShowDemoWindow();
		ImGui::SetNextWindowPos(
			ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f),
			ImGuiCond_Always,
			ImVec2(1.0f, 0.0f));

		ImGui::Begin("Controls", nullptr);

		if (ImGui::Button("Play/Pause"))
		{
			scene.togglePause();
		}
		if (ImGui::Button("Reset"))
		{
			scene.reset();
		}

		if (ImGui::Button(scene.autoReset ? "Turn Off Auto-Reset" : "Turn On Auto-Reset"))
		{
			scene.toggleAutoReset();
		}
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Stats");
		ImGui::BeginChild("Stats");

		ImGui::Text("Board Size: %dx%d", scene.game.width, scene.game.height);
		ImGui::Text("trials: %d", trialCount);
		ImGui::Text("sizes: %d", sizesDone);
		ImGui::Text("moves: %d", totalMoves);
		ImGui::EndChild();

		ImGui::End();
		ImGui::Begin("data");
		float maxY = 0.f;
		for (auto i : avgMoveData)
		{
			if (i > maxY)
			{
				maxY = i;
			}
		}

		ImGui::PlotLines("AVG MOVES", &avgMoveData.front(), avgMoveData.size(), 0, NULL, float(NUM_BOARD_SIZES), maxY, ImVec2(400.f, 400.f), sizeof(avgMoveData.front()));
		ImGui::End();
		window.clear();
		if (!done)
		{
			scene.update();
			if (scene.finished)
			{
				trialCount++;

				totalMoves += scene.game.moveCount;
				if (trialCount == TRIALS_PER_SIZE)
				{

					avgMoveData.push_back(float(totalMoves / TRIALS_PER_SIZE));
					for (auto i : avgMoveData)
					{
						std::cout << i << "\n";
					}

					totalMoves = 0;
					sizesDone++;
					if (sizesDone == NUM_BOARD_SIZES)
					{

						done = true;
					}
					else
					{
						trialCount = 0;
						scene.restart(startw + sizesDone, starth + sizesDone);
					}
				}
				else
				{
					scene.reset();
				}
			}
		}

		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}