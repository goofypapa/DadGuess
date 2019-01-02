#!/bin/bash

echo "update Web_Astronomy ..."
cd Web_Astronomy
git pull

echo "update Web_ABC ..."
cd ../Web_ABC
git pull

echo "update Web_Animal ..."
cd ../Web_Animal
git pull

echo "update Web_WorldHistory ..."
cd ../Web_WorldHistory
git pull

echo "update Web_Earth ..."
cd ../Web_Earth
git pull

echo "update Web_HistoryChronology ..."
cd ../Web_HistoryChronology
git pull

cd ..

