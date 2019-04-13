﻿using System;
using System.Collections.Generic;
using System.Text;

namespace SolarApp.SolarMath
{
    interface ISolar
    {
        double CalculateVoc(double T, double Iph, double Is);
        double CalculateFF(double Im, double Vm, double Isc, double Voc);
        double CalculateN(double PMax, double Pin);
        double CalculatePMax();
        double CalculatePIn(double A, double IR);
        void SetPoints(double V, double I);
    }
}
