
# Operating Systems

Writing a Shell Script to Backup a Directory
## Run using Makefile

## To run this script using Makefile

```bash
  make run dir=? backupdir=? interval-secs=? max-backups=?

```
## Screenshots

### dir is the source directory we want to backup
![source directory we want to backup](https://user-images.githubusercontent.com/82238829/196806396-e43eb622-03d4-43d7-aa3a-5d184a3a055b.png)

### after running the Makefile 

![after running Makefile](https://user-images.githubusercontent.com/82238829/196807592-e2973fd2-15ad-4aeb-ada5-98f02f4f2a0d.png)

### after modifying dir directory files

![file is modified](https://user-images.githubusercontent.com/82238829/196807344-406d9ad4-d4ae-45c6-9fb0-356047900318.png)

### another directory is created contains recent backup

![another directory is created contains recent backup](https://user-images.githubusercontent.com/82238829/196789456-48309796-c679-4e19-900e-934ef583b620.png)

## Run using cron job

## To run this script using cron job every 1 minute run

```bash
  sudo crontab -e # to edit cron job
```

```bash
  # write this in cron job file
  * * * * * /bin/bash shellScriptDirectoryPath sourceDirectoryPath backupDirectoryPath maxBackupDirectories

```

### the same result can be obtained using cron job
![editing cron job file](https://user-images.githubusercontent.com/82238829/196808594-dfe870fc-dbb1-4e76-9933-0cb3ef2e09d2.png)

## What should be the cron expression if I need to run this backup every 3 rd Friday of the month at 12:31 am

```bash
  31 12 21 10 5
  Minute: 31; Hour:12; MonthDay: 21; Month: October; WeekDay: Friday
```
## Documentation

[Documentation](https://drive.google.com/drive/folders/15BfgpnRF_W9Xto8lVHPx7jrcPkhTz8oO?usp=sharing)


## Authors

- [@ahmedtawfiiiq](https://github.com/Ahmedtawfiiiq)
