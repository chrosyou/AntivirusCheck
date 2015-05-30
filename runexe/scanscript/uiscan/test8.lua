--test
GUID = "00000000000000000000000000000000";

--Scan time
local sTime;
--virus version
local sVirusVerison;
--Scan result
local tResult = {};

function GetScanTime(file, sKey)
	local line = file:read("*line");
	while nil ~= line do
		local iStart, iEnd = string.find(line, sKey);
		if nil ~= iStart then
			return string.sub(line, iEnd,-1);
		end
		line = file:read("*line");
	end
end

function AddtoReslult(sFileName, sVirusName)
	for i,v in pairs(tResult) do
		if i == sFileName then
			return
		end
	end
	
	tResult[sFileName] = sVirusName;
end

function ParasResult()
	local file = io.open("c:\\e.txt", "r");
	sTime = GetScanTime(file, "* Started on:");
	sVirusVerison = GetScanTime(file, "* VPS: ");

	
	local line = file:read("*line");
	while nil ~= line do
		local iStart, iEnd = string.find(line, "|");   --zip file 
		if nil ~= iStart then
			local sFileName = string.sub(line, 0, iStart - 1);
			local iStartVirus, iEndVirus = string.find(line, "[L]");
			local sVirusName = string.sub(line, iEndVirus+2, -1);
			AddtoReslult(sFileName, sVirusName);
		else
			local iStartVirus, iEndVirus = string.find(line, "[L]");   --single file
			if nil ~= iStartVirus then
				local sFileName = string.sub(line, 1, iStartVirus - 2);
				local sVirusName = string.sub(line, iEndVirus+2, -1);
				AddtoReslult(sFileName, sVirusName);
			end
		end
		
		line = file:read("*line");
	end
end

function TestPrint()
	oaApi.Print(sTime);
	oaApi.Print(sVirusVerison);
	for i,v in pairs(tResult) do
		oaApi.Print(i.."******"..v);
	end
end

function Scan()
	oaApi.Print("test8");

	--Run Process
	--oaApi.RunPrccess("C:\\Program Files\\AVAST Software\\Avast\\ashCmd.exe", "c:\\test /T=A /P -r=c:\\e.txt", 10000000);
	
	--ParasResult();
	
	--TestPrint();
end

function main(argc, argv)
	if argc ~= 0 and #argv ~= 0 then
		if argv[1] == "Scan" then
			Scan();
			return 20;
		end
	end
end