#pragma once

#include <iostream>
#include <limits>
#include <climits>
#include <src/data_management/data_types.h>

inline void getValidFloat(float *input, int index, float min, float max, const char *prompt)
{
    while (true)
    {
        std::cout << prompt;

        if (std::cin >> input[index])
        {
            if (input[index] >= min and input[index] <= max)
            {
                return;
            }

            std::cout << "Value must be between "
                      << min << " and " << max << ".\n";
        }
        else
        {
            std::cout << "Invalid input. Please enter a number.\n";

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

inline void getValidInt(int *input, int index, int min, int max, const char *prompt)
{
    while (true)
    {
        std::cout << prompt;

        if (std::cin >> input[index])
        {
            if (input[index] >= min and input[index] <= max)
            {
                return;
            }

            std::cout << "Value must be between "
                      << min << " and " << max << ".\n";
        }
        else
        {
            std::cout << "Invalid input. Please enter an integer.\n";

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}
inline void getValidUInt(
    unsigned int *input,
    unsigned int min,
    unsigned int max,
    const char *prompt)
{
    while (true)
    {
        std::cout << prompt;

        if (std::cin >> *input)
        {
            if (*input >= min && *input <= max)
            {
                return;
            }

            std::cout << "Value must be between "
                      << min << " and " << max << ".\n";
        }
        else
        {
            std::cout << "Invalid input. Please enter a positive integer.\n";

            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n');
        }
    }
}

inline void getUserInput(
    float *float_input,
    size_t float_input_size,
    int *int_input,
    size_t int_input_size,
    UserInputCase use_case)
{
    switch (use_case)
    {
    case UserInputCase::Default:
    {
        break;
    }

    case UserInputCase::Visual:
    {
        // FLOAT INPUTS

        if (eRad < float_input_size)
        {
            getValidFloat(
                float_input,
                eRad,
                0.0f,
                1.0f,
                "Enter radiation (0.0 - 1.0): ");
        }

        if (cull < float_input_size)
        {
            getValidFloat(
                float_input,
                cull,
                0.0f,
                1.0f,
                "Enter cull percentage (0.0 - 1.0): ");
        }

        // INT INPUTS

        if (maxA < int_input_size)
        {
            getValidInt(
                int_input,
                maxA,
                0,
                INT_MAX,
                "Enter maximum amount of Agents: ");
        }

        if (maxP < int_input_size)
        {
            getValidInt(
                int_input,
                maxP,
                0,
                INT_MAX,
                "Enter maximum amount of Plants: ");
        }

        if (iT < int_input_size)
        {
            getValidInt(
                int_input,
                iT,
                0,
                INT_MAX,
                "Enter iteration start (number): ");
        }

        if (maxCultivate < int_input_size)
        {
            getValidInt(
                int_input,
                maxCultivate,
                0,
                INT_MAX,
                "Enter max cultivation iterations (number): ");
        }

        if (maxCycle < int_input_size)
        {
            getValidInt(
                int_input,
                maxCycle,
                0,
                INT_MAX,
                "Enter max total cycles (number): ");
        }

        if (cb < int_input_size)
        {
            getValidInt(
                int_input,
                cb,
                0,
                INT_MAX,
                "Enter carbon count (number) (not implemented yet): ");
        }

        if (maxBL < int_input_size)
        {
            getValidInt(
                int_input,
                maxBL,
                1,
                INT_MAX,
                "Enter max brain depth (1+): ");
        }

        if (maxBCN < int_input_size)
        {
            getValidInt(
                int_input,
                maxBCN,
                1,
                INT_MAX,
                "Enter max brain child nodes count (1+): ");
        }

        if (rootNodesCount < int_input_size)
        {
            getValidInt(
                int_input,
                rootNodesCount,
                1,
                INT_MAX,
                "Enter brain root nodes count (1+): ");
        }

        if (fps < int_input_size)
        {
            getValidInt(
                int_input,
                fps,
                1,
                INT_MAX,
                "Enter environment frames per second 1+: ");
        }

        // GLOBAL VISUAL VARIABLES

        getValidUInt(
            &borderWidth,
            2,
            UINT_MAX,
            "Enter visual border width (2+): ");

        getValidUInt(
            &borderHeight,
            2,
            UINT_MAX,
            "Enter visual border height (2+): ");

        getValidUInt(
            &agentSize,
            1,
            UINT_MAX,
            "Enter agent size (1+): ");

        getValidUInt(
            &plantSize,
            1,
            UINT_MAX,
            "Enter plant size (1+): ");

        break;
    }

    case UserInputCase::Multithread:
    {
        // FLOAT INPUTS

        if (eRad < float_input_size)
        {
            getValidFloat(
                float_input,
                eRad,
                0.0f,
                1.0f,
                "Enter radiation (0.0 - 1.0): ");
        }

        if (cull < float_input_size)
        {
            getValidFloat(
                float_input,
                cull,
                0.0f,
                1.0f,
                "Enter cull percentage (0.0 - 1.0): ");
        }

        // INT INPUTS

        if (maxA < int_input_size)
        {
            getValidInt(
                int_input,
                maxA,
                0,
                INT_MAX,
                "Enter maximum amount of Agents: ");
        }

        if (maxP < int_input_size)
        {
            getValidInt(
                int_input,
                maxP,
                0,
                INT_MAX,
                "Enter maximum amount of Plants: ");
        }

        if (iT < int_input_size)
        {
            getValidInt(
                int_input,
                iT,
                0,
                INT_MAX,
                "Enter iteration start (number): ");
        }

        if (maxCultivate < int_input_size)
        {
            getValidInt(
                int_input,
                maxCultivate,
                0,
                INT_MAX,
                "Enter max cultivation iterations (number): ");
        }

        if (maxCycle < int_input_size)
        {
            getValidInt(
                int_input,
                maxCycle,
                0,
                INT_MAX,
                "Enter max total cycles (number): ");
        }

        if (cb < int_input_size)
        {
            getValidInt(
                int_input,
                cb,
                0,
                INT_MAX,
                "Enter carbon count (number) (not implemented yet): ");
        }

        if (maxBL < int_input_size)
        {
            getValidInt(
                int_input,
                maxBL,
                1,
                INT_MAX,
                "Enter max brain depth (1+): ");
        }

        if (maxBCN < int_input_size)
        {
            getValidInt(
                int_input,
                maxBCN,
                1,
                INT_MAX,
                "Enter max brain child nodes count (1+): ");
        }

        if (rootNodesCount < int_input_size)
        {
            getValidInt(
                int_input,
                rootNodesCount,
                1,
                INT_MAX,
                "Enter brain root nodes count (1+): ");
        }

        break;
    }

    case UserInputCase::Custom:
    {
        if (fps < int_input_size)
        {
            getValidInt(
                int_input,
                fps,
                1,
                INT_MAX,
                "Enter environment frames per second 1+: ");
        }

        break;
    }
    }
}