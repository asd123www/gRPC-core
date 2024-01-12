//
// Copyright 2015 gRPC authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef GRPC_SRC_CORE_LIB_URI_URI_PARSER_H
#define GRPC_SRC_CORE_LIB_URI_URI_PARSER_H

#include <grpc/support/port_platform.h>

#include <map>
#include <string>
#include <vector>

/* asd123www:
 * Abseil is an open-source collection of C++ library code designed to augment the C++ standard library.
 * You can find "absl/status/statusor.h" here: https://github.com/abseil/abseil-cpp/blob/master/absl/status/status.h.
 * Looks like gRPC built upon this C++ lib.
 */
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"

namespace grpc_core {

class URI { // URI: Uniform Resource Identifier, rfc 3986.
 public:
  struct QueryParam {
    std::string key;
    std::string value;
    bool operator==(const QueryParam& other) const {
      return key == other.key && value == other.value;
    }
    bool operator<(const QueryParam& other) const {
      int c = key.compare(other.key);
      if (c != 0) return c < 0;
      return value < other.value;
    }
  };

  /* asd123www:
   * absl::StatusOr<T>: returns either T object or "absl:Status", an error handling object.
   * absl::string_view: read-only string, provide more safety.
   */
  // Creates a URI by parsing an rfc3986 URI string. Returns an
  // InvalidArgumentError on failure.
  static absl::StatusOr<URI> Parse(absl::string_view uri_text);
  // Creates a URI from components. Returns an InvalidArgumentError on failure.
  /* GPT-4:
   * std::string scheme: The scheme component of the URI (like http, https, ftp).
   * std::string authority: The authority component, typically including the user information, host, and port.
   * std::string path: The path component of the URI.
   * std::vector<QueryParam> query_parameter_pairs: A vector of query parameters. Each QueryParam is likely a structure or class representing a key-value pair for query parameters.
   * std::string fragment: The fragment component of the URI, used to identify a secondary resource, like a section heading in a document.
   */
  static absl::StatusOr<URI> Create(
      std::string scheme, std::string authority, std::string path,
      std::vector<QueryParam> query_parameter_pairs, std::string fragment);

  URI() = default;

  // Copy construction and assignment
  URI(const URI& other);
  URI& operator=(const URI& other);
  // Move construction and assignment
  URI(URI&&) = default;
  URI& operator=(URI&&) = default;

  // asd123www: percent encoding, also known URL encoding.
  static std::string PercentEncodeAuthority(absl::string_view str);
  static std::string PercentEncodePath(absl::string_view str);

  static std::string PercentDecode(absl::string_view str);

  // asd123www: insertion operator.
  const std::string& scheme() const { return scheme_; }
  const std::string& authority() const { return authority_; }
  const std::string& path() const { return path_; }
  // Stores the *last* value appearing for each repeated key in the query
  // string. If you need to capture repeated query parameters, use
  // `query_parameter_pairs`.
  const std::map<absl::string_view, absl::string_view>& query_parameter_map()
      const {
    return query_parameter_map_;
  }
  // A vector of key:value query parameter pairs, kept in order of appearance
  // within the URI search string. Repeated keys are represented as separate
  // key:value elements.
  const std::vector<QueryParam>& query_parameter_pairs() const {
    return query_parameter_pairs_;
  }
  const std::string& fragment() const { return fragment_; }



  std::string ToString() const;

 private:
  URI(std::string scheme, std::string authority, std::string path,
      std::vector<QueryParam> query_parameter_pairs, std::string fragment);

  std::string scheme_;
  std::string authority_;
  std::string path_;
  std::map<absl::string_view, absl::string_view> query_parameter_map_;
  std::vector<QueryParam> query_parameter_pairs_;
  std::string fragment_;
};
}  // namespace grpc_core

#endif  // GRPC_SRC_CORE_LIB_URI_URI_PARSER_H
