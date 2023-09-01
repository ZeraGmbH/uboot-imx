# if rescue requested boot to internal emmc
if env exists rescuerequested;
then
    if test ${rescuerequested} > 0;
    then
        env set mmcblk 0
		run loaduimage
		run mmcboot
    fi
fi

if env exists slot;
then 
	
else
	setenv slot 2;
	env save;
fi

if env exists mmcrootpartswitched;
then 
	
else
	setenv mmcrootpartswitched 0;
	env save;
fi

if env exists bootcount;
then 

else
	setenv bootcount 0;
	env save;
fi

if env exists updateinprogress;
then
	if test ${updateinprogress} > 0;
	then
		# check if partition already flipped during update process, if not then switch rootfs partitions
		if test ${mmcrootpartswitched} = 0;
		then
			if test $slot = 2;
			then
				echo "Requested partition switch from 2 to 3";
				env set slot 3;
			else
				echo "Requested partition switch from 3 to 2";
				env set slot 2;
			fi
			env set mmcrootpartswitched 1;
			env save;
		fi

		# if 2 boots failed on "new" partition switch back to the old partition
		if test ${bootcount} > 1;
		then
			# rootfs part flip logic
			setenv bootcount 0;
			if test $slot = 2;
			then
				echo "Fallback flipping partition from 2 to 3";
				env set slot 3;
			else
				echo "Fallback flipping partition from 3 to 2";
				env set slot 2;
			fi
			env save;
		else
			setexpr bootcount ${bootcount} + 1;
			echo "boot: increase bootcount";
			env save;
		fi
	elif test ${updateinprogress} = 0 && test ${mmcrootpartswitched} = 1;
	then
		env set mmcrootpartswitched 0;
		env set bootcount 0;
		env save;		
	fi
fi


setenv mmcrootpart $slot

echo "Bootcount: $bootcount"
echo "rootfs partition: $mmcrootpart"
echo "Partition switched: $mmcrootpartswitched"
echo "Update in progress: $updateinprogress"

run loaduimage
run mmcboot
