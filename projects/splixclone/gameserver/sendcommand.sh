#!/bin/bash
echo "$@"  | nc -w 1 -U /tmp/LocalSocketServerTest
