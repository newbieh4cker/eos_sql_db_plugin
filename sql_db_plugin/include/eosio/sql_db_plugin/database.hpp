#pragma once
#include <soci/soci.h>

#include <eosio/chain/config.hpp>
#include <eosio/chain/block_state.hpp>
#include <eosio/chain/transaction.hpp>
#include <eosio/chain/trace.hpp>
#include <eosio/chain/types.hpp>

#include <eosio/sql_db_plugin/accounts_table.hpp>
#include <eosio/sql_db_plugin/transactions_table.hpp>
#include <eosio/sql_db_plugin/blocks_table.hpp>
#include <eosio/sql_db_plugin/actions_table.hpp>
#include <eosio/sql_db_plugin/traces_table.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace eosio {

class database {
    public:
        database(const std::string& uri, uint32_t block_num_start);
        
        void wipe();
        bool is_started();
        void consume_block_state( const chain::block_state_ptr& );
        void consume_irreversible_block_state( const chain::block_state_ptr&, boost::mutex::scoped_lock&, boost::condition_variable&, boost::atomic<bool>& );


        void consume_transaction_metadata( const chain::transaction_metadata_ptr& );
        void consume_transaction_trace( const chain::transaction_trace_ptr& );

        static const std::string block_states_col;
        static const std::string blocks_col;
        static const std::string trans_col;
        static const std::string trans_traces_col;
        static const std::string actions_col;
        static const std::string accounts_col;

    private:
        std::shared_ptr<soci::session> m_session;
        std::unique_ptr<actions_table> m_actions_table;
        std::unique_ptr<accounts_table> m_accounts_table;
        std::unique_ptr<blocks_table> m_blocks_table;
        std::unique_ptr<transactions_table> m_transactions_table;
        std::unique_ptr<traces_table> m_traces_table;
        std::string system_account;
        uint32_t m_block_num_start;
    };

} // namespace

