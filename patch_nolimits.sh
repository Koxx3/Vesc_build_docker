#!/bin/bash
sed -i 's|//#define DISABLE_HW_LIMITS.*|#define DISABLE_HW_LIMITS\n|' ./bldc-503/conf_general.h
sed -i 's|//#define DISABLE_HW_LIMITS.*|#define DISABLE_HW_LIMITS\n|' ./bldc-master/conf_general.h

