//! Raills Network Protocol
//! Return codes

enum Rail_retCode {
    Rail_retCode_RIF_OKsent          =  16,
    Rail_retCode_OKnoReply           =   0,

    Rail_retCode_IncompatibleVersion =  -1,
    Rail_retCode_BadDestinationCount =  -2,
    Rail_retCode_ServiceNotSupported =  -3,
    Rail_retCode_UnknownRIFID        =  -4,

    Rail_retCode_RIF_Down            = -17
};
