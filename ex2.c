        
/* sanity checks */
    if (z->written) {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, APLOGNO(01818), "attempt made to save the session twice, ","session not saved: %s", r->uri);
        return APR_EGENERAL;
    }
        /* encode the session */
    rv = ap_run_session_encode(r, z);
    if (OK != rv) {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, APLOGNO(01820),
        "error while encoding the session, ",
                "session not saved: %s",
            r->uri);
    return APR_EGENERAL;
    }

    /* try the save */
    rv = ap_run_session_save(r, z);
    if (DECLINED == rv) {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, APLOGNO(01821),
            "session is enabled but no session modules have been configured, "
                "session not saved: %s", r->uri);
                return APR_EGENERAL;
}
