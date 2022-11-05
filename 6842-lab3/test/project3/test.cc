        //     for (int j = 0; j < currentCommand.simpleCommands[i].arguments.size() - 4; j++)
        //     {
        //         if (currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 4 - j][0] == '-')
        //             continue;
        //         currentCommand.simpleCommands[i].inputFile.insert(currentCommand.simpleCommands[i].inputFile.begin(), currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 4 - j]);
        //     }
        //     currentCommand.simpleCommands[i].outputFile = currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 2];
        //     modifyFileDescriptorRewrite(currentCommand, i, 2);
        //     execArguments(currentCommand.simpleCommands[i], 3);
        //     continue;
        // }
        // else if (!currentCommand.simpleCommands[i].background && currentCommand.simpleCommands[i].simpleCommandType == "rewrite")
        // {
        //     for (int j = 0; j < currentCommand.simpleCommands[i].arguments.size() - 3; j++)
        //     {
        //         if (currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 3 - j][0] == '-')
        //             continue;
        //         currentCommand.simpleCommands[i].inputFile.insert(currentCommand.simpleCommands[i].inputFile.begin(), currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 3 - j]);
        //     }
        //     currentCommand.simpleCommands[i].outputFile = currentCommand.simpleCommands[i].arguments[currentCommand.simpleCommands[i].arguments.size() - 1];
        //     modifyFileDescriptorRewrite(currentCommand, i, 1);
        //     execArguments(currentCommand.simpleCommands[i], 2);
        //     continue;
        // }