# Algolia's c++ software engineer technical test

## Building

`mkdir build && cd build && cmake ..`
`make`

## Usage

`./logsearcher [/PATH/TO/LOGS]`

The logs are supposed in TSV format. Each row must be composed as follow:
* `DATE\tQUERY` where DATE is formated as: `%Y-%m-%d %R:%S` in strftime format

## Commands

This software is a command line tool. It means that once you have started
it, you will need to issue commands in order to interact with the datas.

TIMERANGE format is represented as: `[%Y-%m-%d[:%R:%S]].[%Y-%m-%d[:%R:%S]]`
The dates do not have to be present in the file. The left bound is *not less*
than the given one and inclusive. The right bound is *greater* and not
inclusive.
 
The commands are:
* `help` Show help for all options.
* `quit` Quit the command line interface.
* `file PATH/TO/LOGS` Load a new log file.
* `top N [TIMERANGE]` Output the top N popular queries (one per line) that have
                      been done during a specific time range.
* `count [TIMERANGE]` Output the number of distinct queries that have been done
                      during a specific time range.

## Examples 

* `top 10` and `top 10 .` will output the top 10 popular queries. 
* `top 100 2015-08-03.` will output the 100 popular queries that have been done
after August 3rd, 2015.
* `count .2015-08-03:01:02:03` will output the number of queries that have been
done after August 3rd, 2015 01:02:03.
* `count 2015-03-03:00:00:00.2015-05-04` will output the number of queries that
have been done beetwen March 3rd, 2015 and May 4th, 2015.

## Authors

Alexandre 'Dot-H' Bernard (alexandre.bernard@lse.epita.fr)
