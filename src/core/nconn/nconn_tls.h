//: ----------------------------------------------------------------------------
//: Copyright (C) 2014 Verizon.  All Rights Reserved.
//: All Rights Reserved
//:
//: \file:    nconn_tls.h
//: \details: TODO
//: \author:  Reed P. Morrison
//: \date:    02/07/2014
//:
//:   Licensed under the Apache License, Version 2.0 (the "License");
//:   you may not use this file except in compliance with the License.
//:   You may obtain a copy of the License at
//:
//:       http://www.apache.org/licenses/LICENSE-2.0
//:
//:   Unless required by applicable law or agreed to in writing, software
//:   distributed under the License is distributed on an "AS IS" BASIS,
//:   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//:   See the License for the specific language governing permissions and
//:   limitations under the License.
//:
//: ----------------------------------------------------------------------------
#ifndef _NCONN_TLS_H
#define _NCONN_TLS_H

//: ----------------------------------------------------------------------------
//: Includes
//: ----------------------------------------------------------------------------
#include "nconn_tcp.h"
#include "ndebug.h"
#include <openssl/ssl.h>

//: ----------------------------------------------------------------------------
//: Constants
//: ----------------------------------------------------------------------------

namespace ns_hlx {

//: ----------------------------------------------------------------------------
//: Fwd Decl's
//: ----------------------------------------------------------------------------

//: ----------------------------------------------------------------------------
//: Enums
//: ----------------------------------------------------------------------------


//: ----------------------------------------------------------------------------
//: \details: TODO
//: ----------------------------------------------------------------------------
class nconn_tls: public nconn_tcp
{
public:
        // ---------------------------------------
        // Options
        // ---------------------------------------
        typedef enum tls_opt_enum
        {
                OPT_TLS_CTX = 1000,

                // Settings
                OPT_TLS_CIPHER_STR = 1001,
                OPT_TLS_OPTIONS = 1002,
                OPT_TLS_SSL = 1003,

                // Verify options
                OPT_TLS_VERIFY = 1100,
                OPT_TLS_SNI = 1102,
                OPT_TLS_VERIFY_ALLOW_SELF_SIGNED = 1103,

                // CA options
                OPT_TLS_CA_FILE = 1201,
                OPT_TLS_CA_PATH = 1202,

                // Server config
                OPT_TLS_TLS_KEY = 1301,
                OPT_TLS_TLS_CRT = 1302,

                OPT_TLS_SENTINEL = 1999

        } tls_opt_t;

        // ---------------------------------------
        // Public methods
        // ---------------------------------------
        nconn_tls():
          nconn_tcp(),
          m_tls_ctx(NULL),
          m_tls(NULL),
          m_tls_opt_verify(false),
          m_tls_opt_verify_allow_self_signed(false),
          m_tls_opt_tlsext_hostname(""),
          m_tls_opt_ca_file(""),
          m_tls_opt_ca_path(""),
          m_tls_opt_options(0),
          m_tls_opt_cipher_str(""),
          m_tls_key(""),
          m_tls_crt(""),
          m_tls_state(TLS_STATE_FREE)
          {
                m_scheme = SCHEME_TLS;
          };

        // Destructor
        ~nconn_tls() {};
        int32_t set_opt(uint32_t a_opt, const void *a_buf, uint32_t a_len);
        int32_t get_opt(uint32_t a_opt, void **a_buf, uint32_t *a_len);
        bool is_listening(void) {return (m_tls_state == TLS_STATE_LISTENING);};
        bool is_connecting(void) {return ((m_tls_state == TLS_STATE_CONNECTING) ||
                                          (m_tls_state == TLS_STATE_TLS_CONNECTING) ||
                                          (m_tls_state == TLS_STATE_TLS_CONNECTING_WANT_READ) ||
                                          (m_tls_state == TLS_STATE_TLS_CONNECTING_WANT_WRITE));};
        bool is_accepting(void) {return ((m_tls_state == TLS_STATE_ACCEPTING) ||
                                         (m_tls_state == TLS_STATE_TLS_ACCEPTING) ||
                                         (m_tls_state == TLS_STATE_TLS_ACCEPTING_WANT_READ) ||
                                         (m_tls_state == TLS_STATE_TLS_ACCEPTING_WANT_WRITE));};
        bool is_free(void) { return (m_tls_state == TLS_STATE_FREE);}

protected:
        // -------------------------------------------------
        // Protected methods
        // -------------------------------------------------
        int32_t ncsetup(evr_loop *a_evr_loop);
        int32_t ncread(evr_loop *a_evr_loop, char *a_buf, uint32_t a_buf_len);
        int32_t ncwrite(evr_loop *a_evr_loop, char *a_buf, uint32_t a_buf_len);
        int32_t ncaccept(evr_loop *a_evr_loop);
        int32_t ncconnect(evr_loop *a_evr_loop);
        int32_t nccleanup(void);
        int32_t ncset_listening(evr_loop *a_evr_loop, int32_t a_val);
        int32_t ncset_listening_nb(evr_loop *a_evr_loop, int32_t a_val);
        int32_t ncset_accepting(evr_loop *a_evr_loop, int a_fd);

private:
        // ---------------------------------------
        // Connection state
        // ---------------------------------------
        typedef enum tls_state
        {
                TLS_STATE_FREE = 0,
                TLS_STATE_LISTENING,
                TLS_STATE_CONNECTING,
                TLS_STATE_ACCEPTING,

                // Connecting
                TLS_STATE_TLS_CONNECTING,
                TLS_STATE_TLS_CONNECTING_WANT_READ,
                TLS_STATE_TLS_CONNECTING_WANT_WRITE,

                // Accepting
                TLS_STATE_TLS_ACCEPTING,
                TLS_STATE_TLS_ACCEPTING_WANT_READ,
                TLS_STATE_TLS_ACCEPTING_WANT_WRITE,

                TLS_STATE_CONNECTED,
                TLS_STATE_READING,
                TLS_STATE_WRITING,
                TLS_STATE_DONE
        } tls_state_t;

        // -------------------------------------------------
        // Private methods
        // -------------------------------------------------
        DISALLOW_COPY_AND_ASSIGN(nconn_tls)

        int32_t tls_connect(void);
        int32_t tls_accept(void);
        int32_t init(void);

        // -------------------------------------------------
        // Private members
        // -------------------------------------------------
        SSL_CTX * m_tls_ctx;
        SSL *m_tls;
        bool m_tls_opt_verify;
        bool m_tls_opt_verify_allow_self_signed;
        std::string m_tls_opt_tlsext_hostname;
        std::string m_tls_opt_ca_file;
        std::string m_tls_opt_ca_path;
        long m_tls_opt_options;
        std::string m_tls_opt_cipher_str;
        std::string m_tls_key;
        std::string m_tls_crt;
        tls_state_t m_tls_state;
};

} //namespace ns_hlx {

#endif
