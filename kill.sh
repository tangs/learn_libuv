#!/bin/bash

ps -ef | grep './out/server.out' | grep -v "grep" | awk '{print$2}' | xargs kill -9
ps -ef | grep './out/client.out' | grep -v "grep" | awk '{print$2}' | xargs kill -9
