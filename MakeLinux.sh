if [ ! -d Linux_Exe ]; then
  echo "Creating output directory" 
  mkdir -p Linux_Exe;
fi
echo "Compiling Idem"
g++ -I..\..\Libs\Units\Headers -o Linux_Exe/idem "..\..\Libs\Units\Source\SlyvArgParse.cpp" "..\..\Libs\Units\Source\SlyvBank.cpp" "..\..\Libs\Units\Source\SlyvDir.cpp" "..\..\Libs\Units\Source\SlyvMD5.cpp" "..\..\Libs\Units\Source\SlyvSTOI.cpp" "..\..\Libs\Units\Source\SlyvStream.cpp" "..\..\Libs\Units\Source\SlyvString.cpp" "..\..\Libs\Units\Source\SlyvTime.cpp" "..\..\Libs\Units\Source\SlyvVolumes.cpp" "Idem.cpp" 
echo "Sequence ended"