--test
GUID = "00000000000000000000000000000000";

function Scan()
	return 2;
end

function main(argc, argv)
	if argc ~= 0 and #argv ~= 0 then
		if argv[1] == "Scan" then
			Scan();
		end
	end
end