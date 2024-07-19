    if (option & AP_CTIME_OPTION_COMPACT) {
        int real_month = xt.tm_mon + 1;
        date_str[7]  = real_year / 1000 + '0';
        date_str[8]  = real_year % 1000 / 100 + '0';
        date_str[9]  = real_year % 100 / 10 + '0';
        date_str[10] = real_year % 10 + '0';
        date_str[11] = '-';
        date_str[12] = real_month / 10 + '0';
        date_str[13] = real_month % 10 + '0';
        date_str[14] = '-';
    }
    else {
        s = &apr_day_snames[xt.tm_wday][0];
        date_str[7]  = s[0];
        date_str[8]  = s[1];
        date_str[9]  = s[2];
        date_str[10] = ' ';
        s = &apr_month_snames[xt.tm_mon][0];
        date_str[11] = s[0];
        date_str[12] = s[1];
        date_str[13] = s[2];
        date_str[14] = ' ';
    }
