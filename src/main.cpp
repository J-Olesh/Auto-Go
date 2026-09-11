#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <GoScene.h>
#include <iostream>
#include <fstream>
#include <string>

int main()
{

	std::fstream file;
	file.open("../../results.csv", std::ios::app);
	if (!file.is_open())
	{
		std::cerr << "Failed to open file!" << std::endl;
		return 1;
	}
	std::vector<std::vector<std::string>> outputData;
	// outputData.push_back({"board size", "moves", "captures (B)", "captures (W)"});

	sf::RenderWindow window(sf::VideoMode({1000, 800}), "ImGui + SFML = <3");
	// window.setFramerateLimit(60);

	ImGui::SFML::Init(window);
	std::vector<float> avgMoveData;

	const int TRIALS_PER_SIZE = 30000;
	int sizesDone = 0;
	int trialCount = 0;
	int totalMoves = 0;
	int maxW = 7;
	int maxH = 7;
	int startw = 7;
	int starth = 7;
	int blackCaptures = 0;
	int whiteCaptures = 0;
	const int NUM_BOARD_SIZES = maxW + 1 - startw;
	bool done = false;
	GoScene scene = GoScene(window, startw, starth);
	scene.enableRender = false;
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
				blackCaptures += scene.game.blackCaptures;
				whiteCaptures += scene.game.whiteCaptures;

				if (trialCount == TRIALS_PER_SIZE)
				{

					avgMoveData.push_back(float(totalMoves / TRIALS_PER_SIZE));
					outputData.push_back({std::to_string(scene.game.height),
										  std::to_string(scene.game.width * scene.game.height),
										  std::to_string(float(totalMoves / TRIALS_PER_SIZE)),
										  std::to_string(float((totalMoves / TRIALS_PER_SIZE)) / (scene.game.width * scene.game.height)),
										  std::to_string(blackCaptures / TRIALS_PER_SIZE),
										  std::to_string(whiteCaptures / TRIALS_PER_SIZE)});

					totalMoves = 0;
					blackCaptures = 0;
					whiteCaptures = 0;
					sizesDone++;
					if (sizesDone == NUM_BOARD_SIZES)
					{

						for (auto &row : outputData)
						{
							for (auto it = row.begin(); it != row.end(); ++it)
							{
								if (std::next(it) != row.end())
								{
									file << *it << ", ";
								}
								else
								{
									file << *it;
								}
							}
							file << "\n";
						}
						file.close();
						std::cout << "CSV file created successfully." << std::endl;
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