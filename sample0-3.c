    #define AP_LOG_RERROR( code, msg ) \
        ap_log_rerror(APLOG_MARK, APLOG_ERR, APLOGNO(code), (msg), \
        "session not saved: %s", r->uri )
    
    /* sanity checks */
    if (z->written) {
        AP_LOG_RERROR( 01818, "attempt made to save the session twice, " );
        return APR_EGENERAL;
    }

    /* encode the session */
    rv = ap_run_session_encode(r, z);
    if (OK != rv) {
        AP_LOG_RERROR( 01820, "error while encoding the session, " );
        return APR_EGENERAL;
    }

    /* try the save */
    rv = ap_run_session_save(r, z);
    if (DECLINED == rv) {
        AP_LOG_RERROR( 01821, "session is enabled but no session modules have been configured, " );
        return APR_EGENERAL;
    }
