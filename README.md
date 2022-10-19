
# Operating Systems

Writing a Shell Script to Backup a Directory
## Run using Makefile

To run this script using Makefile run

```bash
  make run dir=? backupdir=? interval-secs=? max-backups=?

```
## Screenshots

### dir is the source directory we want to backup
![source directory we want to backup](https://user-images.githubusercontent.com/82238829/196786345-bf48d0a3-03e9-4d18-aa86-fb35979d0711.png)

### after running the Makefile 

![after running Makefile](https://user-images.githubusercontent.com/82238829/196787566-511575c3-8440-4405-b20e-efdbd35997c3.png)

### after modifying dir directory files

![file is modified](https://user-images.githubusercontent.com/82238829/196789177-7b5a3097-add4-4ee8-ac45-c51eef1167fb.png)

### another directory is created contains recent backup

![another directory is created contains recent backup](https://user-images.githubusercontent.com/82238829/196789456-48309796-c679-4e19-900e-934ef583b620.png)

To run this script using cron job every 1 minute run

## Run using cron job
```bash
  sudo crontab -e # to edit cron job
```

```bash
  # write this in cron job file
  * * * * * /bin/bash shellScriptDirectoryPath sourceDirectoryPath backupDirectoryPath timeInterval maxBackupDirectories

```

### the same result can be obtained using cron job
![editing cron job file](https://user-images.githubusercontent.com/82238829/196791107-6e468eb1-a452-4bea-804e-b9ae9a3d4e99.png)

## What should be the cron expression if I need to run this backup every 3 rd Friday of the month at 12:31 am

```bash
  31 12 21 10 5
  Minute: 31; Hour:12; MonthDay: 21; Month: October; WeekDay: Friday
```
## Documentation

[Documentation](https://drive.google.com/drive/folders/15BfgpnRF_W9Xto8lVHPx7jrcPkhTz8oO?usp=sharing)


## Authors

- [@ahmedtawfiiiq](https://github.com/Ahmedtawfiiiq)
