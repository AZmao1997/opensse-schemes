//
//  large_storage_sophos_client.hpp
//  sophos
//
//  Created by Raphael Bost on 13/04/2016.
//  Copyright © 2016 Raphael Bost. All rights reserved.
//

#pragma once

#include "sophos_core.hpp"

#include <array>

namespace sse {
    namespace sophos {
        
        class MediumStorageSophosClient : public SophosClient {
        public:
            static constexpr size_t kKeywordIndexSize = 16;
            typedef std::array<uint8_t, kKeywordIndexSize> keyword_index_type;
            
            static std::unique_ptr<SophosClient> construct_from_directory(const std::string& dir_path);
            static std::unique_ptr<SophosClient> init_in_directory(const std::string& dir_path, uint32_t n_keywords);

            
            static std::unique_ptr<SophosClient> construct_from_json(const std::string& dir_path, const std::string& json_path);
            
            MediumStorageSophosClient(const std::string& token_map_path, const size_t tm_setup_size);
            MediumStorageSophosClient(const std::string& token_map_path, const std::string& tdp_private_key, const std::string& derivation_master_key, const std::string& rsa_prg_key);
            MediumStorageSophosClient(const std::string& token_map_path, const std::string& tdp_private_key, const std::string& derivation_master_key, const std::string& rsa_prg_key, const size_t tm_setup_size);
            ~MediumStorageSophosClient();
            
            size_t keyword_count() const;
            
            SearchRequest   search_request(const std::string &keyword) const;
            UpdateRequest   update_request(const std::string &keyword, const index_type index);
            
            SearchRequest   random_search_request() const;

            
            std::string rsa_prg_key() const;
            
            void write_keys(const std::string& dir_path) const;
            
            std::ostream& db_to_json(std::ostream& out) const;
            std::ostream& print_stats(std::ostream& out) const;
            
            struct IndexHasher
            {
            public:
                size_t operator()(const keyword_index_type& ind) const;
            };

        private:
            static const std::string rsa_prg_key_file__;
            static const std::string counter_map_file__;

            class JSONHandler;
            friend JSONHandler;
            
            keyword_index_type get_keyword_index(const std::string &kw) const;
            
            crypto::Prf<crypto::Tdp::kRSAPrgSize> rsa_prg_;
            
            ssdmap::bucket_map< keyword_index_type, uint32_t, IndexHasher> counter_map_;
            std::mutex token_map_mtx_;
            std::atomic_uint keyword_counter_;
        };
    }
}