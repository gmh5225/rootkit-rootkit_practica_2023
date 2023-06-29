#!/bin/bash

# Directorul în care sunt stocate aplicațiile
app_directory="/usr/bin"

# Obține lista de aplicații instalate și selectează aleatoriu una
select_random_application() {
  local installed_apps=($(ls $app_directory))
  local num_apps=${#installed_apps[@]}
  local random_index=$((RANDOM % num_apps))
  local random_app=${installed_apps[random_index]}
  echo "$random_app"
}

# Deschide o aplicație aleatoare
open_random_application() {
  local app=$(select_random_application)
  $app &
}

# Numărul de aplicații pe care doriți să le deschideți
num_applications=5

# Deschide aplicațiile aleatoare
for ((i=1; i<=num_applications; i++)); do
  open_random_application
done

