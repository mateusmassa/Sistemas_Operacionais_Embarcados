#!/bin/bash
cp /tmp/users_database.csv /home/victor/Documents/github/3FA_Server/
cd /home/victor/Documents/github/3FA_Server/
git add .
git commit -m "Update repository"
sudo -u victor git push
