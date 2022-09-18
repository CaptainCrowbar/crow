// This file is generated by the rs-update-tests script

#include "crow/unit-test.hpp"

int main(int argc, char** argv) {

    Crow::UnitTest::begin_tests(argc, argv);

    // log-test.cpp
    UNIT_TEST(crow_logging)
    UNIT_TEST(crow_logging_output)

    // types-test.cpp
    UNIT_TEST(crow_types_mixins)
    UNIT_TEST(crow_types_traits)
    UNIT_TEST(crow_types_iterator_category)
    UNIT_TEST(crow_types_range_category)

    // algorithm-test.cpp
    UNIT_TEST(crow_algorithm_binomial_coefficients)
    UNIT_TEST(crow_algorithm_integer_power)
    UNIT_TEST(crow_algorithm_integer_power_generalised)
    UNIT_TEST(crow_algorithm_container_algorithms)
    UNIT_TEST(crow_algorithm_diff)
    UNIT_TEST(crow_algorithm_edit_distance)
    UNIT_TEST(crow_algorithm_hash_compare)
    UNIT_TEST(crow_algorithm_interpolation)
    UNIT_TEST(crow_algorithm_interpolated_map)
    UNIT_TEST(crow_algorithm_interpolated_map_lin_log)
    UNIT_TEST(crow_algorithm_interpolated_map_log_log)
    UNIT_TEST(crow_algorithm_cubic_spline_map)
    UNIT_TEST(crow_algorithm_precision_sum)
    UNIT_TEST(crow_algorithm_line_integral)
    UNIT_TEST(crow_algorithm_volume_integral)
    UNIT_TEST(crow_algorithm_find_optimum)
    UNIT_TEST(crow_algorithm_subsets)

    // binary-test.cpp
    UNIT_TEST(crow_binary_byte_order)
    UNIT_TEST(crow_binary_birwise_operations)
    UNIT_TEST(crow_binary_signed_overflow_detection)
    UNIT_TEST(crow_binary_unsigned_overflow_detection)

    // bounded-array-construction-test.cpp
    UNIT_TEST(crow_bounded_array_construction)

    // bounded-array-insertion-test.cpp
    UNIT_TEST(crow_bounded_array_insertion)

    // bounded-array-misc-test.cpp
    UNIT_TEST(crow_bounded_array_capacity)
    UNIT_TEST(crow_bounded_array_keys)

    // bounded-array-tracking-test.cpp
    UNIT_TEST(crow_bounded_array_tracking)

    // channel-classes-test.cpp
    UNIT_TEST(crow_channel_generator)
    UNIT_TEST(crow_channel_buffer)
    UNIT_TEST(crow_channel_queue)
    UNIT_TEST(crow_channel_value)
    UNIT_TEST(crow_channel_timer)

    // channel-dispatch-test.cpp
    UNIT_TEST(crow_channel_dispatch_empty)
    UNIT_TEST(crow_channel_dispatch_sync_close)
    UNIT_TEST(crow_channel_dispatch_async_close)
    UNIT_TEST(crow_channel_dispatch_sync_exception)
    UNIT_TEST(crow_channel_dispatch_async_exception)
    UNIT_TEST(crow_channel_dispatch_multiple_sync_exception)
    UNIT_TEST(crow_channel_dispatch_multiple_async_exception)
    UNIT_TEST(crow_channel_dispatch_heterogeneous_sync_exception)
    UNIT_TEST(crow_channel_dispatch_heterogeneous_async_exception)
    UNIT_TEST(crow_channel_dispatch_async_message_channel)
    UNIT_TEST(crow_channel_dispatch_async_stream_channel)

    // colour-alpha-test.cpp
    UNIT_TEST(crow_colour_channel_order)
    UNIT_TEST(crow_colour_premultiplied_alpha)
    UNIT_TEST(crow_colour_alpha_blending)

    // colour-conversion-test.cpp
    UNIT_TEST(crow_colour_conversion_between_colour_spaces)
    UNIT_TEST(crow_colour_conversion_between_representations)

    // colour-floating-channel-test.cpp
    UNIT_TEST(crow_colour_floating_point_elements)
    UNIT_TEST(crow_colour_floating_point_arithmetic)
    UNIT_TEST(crow_colour_floating_point_standard_colours)

    // colour-integer-channel-test.cpp
    UNIT_TEST(crow_colour_integral_elements)
    UNIT_TEST(crow_colour_integral_standard_colours)
    UNIT_TEST(crow_colour_integral_arithmetic)

    // colour-interpolation-test.cpp
    UNIT_TEST(crow_colour_interpolation)

    // colour-space-test.cpp
    UNIT_TEST(crow_colour_space_ciexyy)
    UNIT_TEST(crow_colour_space_cielab)
    UNIT_TEST(crow_colour_space_cieluv)
    UNIT_TEST(crow_colour_space_hclab)
    UNIT_TEST(crow_colour_space_hcluv)
    UNIT_TEST(crow_colour_space_srgb)
    UNIT_TEST(crow_colour_space_adobe_rgb)
    UNIT_TEST(crow_colour_space_pro_photo)
    UNIT_TEST(crow_colour_space_wide_gamut)
    UNIT_TEST(crow_colour_space_hsl)
    UNIT_TEST(crow_colour_space_hsv)
    UNIT_TEST(crow_colour_space_greyscale)
    UNIT_TEST(crow_colour_space_sgreyscale)
    UNIT_TEST(crow_colour_space_conversion)

    // colour-string-test.cpp
    UNIT_TEST(crow_colour_hex_representation)
    UNIT_TEST(crow_colour_css_colours)

    // compact-array-construction-test.cpp
    UNIT_TEST(crow_compact_array_construction)

    // compact-array-insertion-test.cpp
    UNIT_TEST(crow_compact_array_insertion)

    // compact-array-misc-test.cpp
    UNIT_TEST(crow_compact_array_capacity)
    UNIT_TEST(crow_compact_array_keys)

    // compact-array-tracking-test.cpp
    UNIT_TEST(crow_compact_array_tracking)

    // constants-test.cpp
    UNIT_TEST(crow_constants)

    // dice-test.cpp
    UNIT_TEST(crow_dice_arithmetic)
    UNIT_TEST(crow_dice_statistics)
    UNIT_TEST(crow_dice_parser)
    UNIT_TEST(crow_dice_generation)
    UNIT_TEST(crow_dice_literals)
    UNIT_TEST(crow_dice_pdf)
    UNIT_TEST(crow_dice_integer_arithmetic)
    UNIT_TEST(crow_dice_integer_statistics)
    UNIT_TEST(crow_dice_integer_parser)
    UNIT_TEST(crow_dice_integer_generation)
    UNIT_TEST(crow_dice_integer_literals)

    // dso-test.cpp
    UNIT_TEST(crow_dynamic_library)

    // encoding-test.cpp
    UNIT_TEST(crow_encoding_hexcode)
    UNIT_TEST(crow_encoding_base64)

    // english-test.cpp
    UNIT_TEST(crow_english_extended_titlecase)
    UNIT_TEST(crow_english_sentence_case)
    UNIT_TEST(crow_english_list_formatting)
    UNIT_TEST(crow_english_number_formatting)
    UNIT_TEST(crow_english_pluralization)
    UNIT_TEST(crow_english_lorem_ipsum)

    // enum-test.cpp
    UNIT_TEST(crow_format_enum_definition)
    UNIT_TEST(crow_format_enum_bitmask_operators)

    // fixed-binary-large-binary-100-test.cpp
    UNIT_TEST(crow_fixed_binary_large_binary_100)

    // fixed-binary-large-binary-35-test.cpp
    UNIT_TEST(crow_fixed_binary_large_binary_35)

    // fixed-binary-misc-test.cpp
    UNIT_TEST(crow_fixed_binary_implementation_selection)
    UNIT_TEST(crow_fixed_binary_type_conversions)
    UNIT_TEST(crow_fixed_binary_string_parsing)
    UNIT_TEST(crow_fixed_binary_hash_set)

    // fixed-binary-small-binary-35-test.cpp
    UNIT_TEST(crow_fixed_binary_small_binary_35)

    // fixed-binary-small-binary-5-test.cpp
    UNIT_TEST(crow_fixed_binary_small_binary_5)

    // font-test.cpp
    UNIT_TEST(crow_font_loading)
    UNIT_TEST(crow_font_properties)
    UNIT_TEST(crow_font_metrics)
    UNIT_TEST(crow_font_text_metrics)
    UNIT_TEST(crow_font_text_fitting)
    UNIT_TEST(crow_font_text_wrapping)
    UNIT_TEST(crow_font_rendering)
    UNIT_TEST(crow_font_map)

    // format-floating-test.cpp
    UNIT_TEST(crow_floating_pointing_point_significant_digits_format)
    UNIT_TEST(crow_floating_pointing_point_scientific_notation)
    UNIT_TEST(crow_floating_pointing_point_fixed_point_format)
    UNIT_TEST(crow_floating_pointing_point_general_format)
    UNIT_TEST(crow_floating_pointing_point_probability_format)

    // format-integer-test.cpp
    UNIT_TEST(crow_signed_integers)
    UNIT_TEST(crow_unsigned_integers)

    // format-null-test.cpp
    UNIT_TEST(crow_null_values)

    // format-numeric-test.cpp
    UNIT_TEST(crow_integer_delimiters)
    UNIT_TEST(crow_floating_point_delimiters)

    // format-print-test.cpp
    UNIT_TEST(crow_print)

    // format-range-test.cpp
    UNIT_TEST(crow_string)
    UNIT_TEST(crow_ranges)

    // format-type-test.cpp
    UNIT_TEST(crow_custom_formatting)
    UNIT_TEST(crow_boolean)
    UNIT_TEST(crow_characters_and_strings)
    UNIT_TEST(crow_pointers)
    UNIT_TEST(crow_types)
    UNIT_TEST(crow_formatter_class)
    UNIT_TEST(crow_function)
    UNIT_TEST(crow_literal)

    // formula-test.cpp
    UNIT_TEST(crow_formula_sphere_geometry)
    UNIT_TEST(crow_formula_mass_density)
    UNIT_TEST(crow_formula_surface_gravity)
    UNIT_TEST(crow_formula_orbital_period)
    UNIT_TEST(crow_formula_hill_sphere)
    UNIT_TEST(crow_formula_stellar_luminosity)
    UNIT_TEST(crow_formula_stellar_lifetime)
    UNIT_TEST(crow_formula_schwarzschild_radius)
    UNIT_TEST(crow_formula_magnitude_luminosity)
    UNIT_TEST(crow_formula_bolometric_correction)
    UNIT_TEST(crow_formula_planet_temperature)

    // geometry-test.cpp
    UNIT_TEST(crow_geometry_box)
    UNIT_TEST(crow_geometry_sphere)

    // guard-test.cpp
    UNIT_TEST(crow_scope_guards)

    // hash-test.cpp
    UNIT_TEST(crow_hash_siphash)
    UNIT_TEST(crow_hash_md5)
    UNIT_TEST(crow_hash_sha1)
    UNIT_TEST(crow_hash_sha256)
    UNIT_TEST(crow_hash_sha512)

    // hexmap-art-test.cpp
    UNIT_TEST(crow_hexmap_ascii_art)
    UNIT_TEST(crow_hexmap_emoji_art)
    UNIT_TEST(crow_hexmap_xterm_art)

    // hexmap-building-test.cpp
    UNIT_TEST(crow_hexmap_adjacency_properties)
    UNIT_TEST(crow_hexmap_initializer_list)

    // hexmap-geometry-test.cpp
    UNIT_TEST(crow_hexmap_coordinate_operations)

    // image-io-test.cpp
    UNIT_TEST(crow_image_io_file_info)
    UNIT_TEST(crow_image_io_load)
    UNIT_TEST(crow_image_io_save)

    // image-resize-test.cpp
    UNIT_TEST(crow_image_resize_dimensions)
    UNIT_TEST(crow_image_resize_content)

    // image-test.cpp
    UNIT_TEST(crow_image_construction)
    UNIT_TEST(crow_image_pixel_access)
    UNIT_TEST(crow_image_premultiplied_alpha)
    UNIT_TEST(crow_image_conversion)

    // index-table-test.cpp
    UNIT_TEST(crow_index_table_classes)

    // interval-continuous-arithmetic-test.cpp
    UNIT_TEST(crow_interval_continuous_contains_zero)
    UNIT_TEST(crow_interval_continuous_interval_reciprocal)
    UNIT_TEST(crow_interval_continuous_interval_arithmetic)

    // interval-continuous-basic-test.cpp
    UNIT_TEST(crow_interval_continuous_interval_basic_properties)
    UNIT_TEST(crow_interval_continuous_interval_construction)
    UNIT_TEST(crow_interval_continuous_interval_from_string)
    UNIT_TEST(crow_interval_continuous_interval_order)
    UNIT_TEST(crow_interval_continuous_interval_complement)
    UNIT_TEST(crow_interval_continuous_interval_binary_operations)
    UNIT_TEST(crow_interval_continuous_interval_hashing)

    // interval-continuous-boundary-addition-test.cpp
    UNIT_TEST(crow_interval_continuous_boundary_inversion)
    UNIT_TEST(crow_interval_continuous_boundary_addition)
    UNIT_TEST(crow_interval_continuous_boundary_subtraction)

    // interval-continuous-boundary-basic-test.cpp
    UNIT_TEST(crow_interval_continuous_boundary_formatting)
    UNIT_TEST(crow_interval_continuous_boundary_from_interval)
    UNIT_TEST(crow_interval_continuous_boundary_to_interval)

    // interval-continuous-boundary-comparison-test.cpp
    UNIT_TEST(crow_interval_continuous_boundary_adjacency)
    UNIT_TEST(crow_interval_continuous_boundary_comparison)

    // interval-continuous-boundary-multiplication-test.cpp
    UNIT_TEST(crow_interval_continuous_boundary_multiplication)

    // interval-continuous-map-test.cpp
    UNIT_TEST(crow_interval_continuous_interval_map)
    UNIT_TEST(crow_interval_continuous_interval_map_hashing)

    // interval-continuous-set-test.cpp
    UNIT_TEST(crow_interval_continuous_interval_set_construct_insert_erase)
    UNIT_TEST(crow_interval_continuous_interval_set_operations)
    UNIT_TEST(crow_interval_continuous_interval_set_hashing)

    // interval-integral-arithmetic-test.cpp
    UNIT_TEST(crow_interval_integral_contains_zero)
    UNIT_TEST(crow_interval_integral_interval_arithmetic)

    // interval-integral-basic-test.cpp
    UNIT_TEST(crow_interval_integral_interval_basic_properties)
    UNIT_TEST(crow_interval_integral_interval_construction)
    UNIT_TEST(crow_interval_integral_interval_from_string)
    UNIT_TEST(crow_interval_integral_interval_iterators)
    UNIT_TEST(crow_interval_integral_interval_order)
    UNIT_TEST(crow_interval_integral_interval_complement)
    UNIT_TEST(crow_interval_integral_interval_binary_operations)
    UNIT_TEST(crow_interval_integral_interval_hashing)

    // interval-integral-boundary-addition-test.cpp
    UNIT_TEST(crow_interval_integral_boundary_inversion)
    UNIT_TEST(crow_interval_integral_boundary_addition)
    UNIT_TEST(crow_interval_integral_boundary_subtraction)

    // interval-integral-boundary-basic-test.cpp
    UNIT_TEST(crow_interval_integral_boundary_formatting)
    UNIT_TEST(crow_interval_integral_boundary_from_interval)
    UNIT_TEST(crow_interval_integral_boundary_to_interval)

    // interval-integral-boundary-comparison-test.cpp
    UNIT_TEST(crow_interval_integral_boundary_adjacency)
    UNIT_TEST(crow_interval_integral_boundary_comparison)

    // interval-integral-boundary-multiplication-test.cpp
    UNIT_TEST(crow_interval_integral_boundary_multiplication)

    // interval-integral-map-test.cpp
    UNIT_TEST(crow_interval_integral_interval_map)
    UNIT_TEST(crow_interval_integral_interval_map_hashing)

    // interval-integral-set-test.cpp
    UNIT_TEST(crow_interval_integral_interval_set_construct_insert_erase)
    UNIT_TEST(crow_interval_integral_interval_set_operations)
    UNIT_TEST(crow_interval_integral_interval_set_hashing)

    // interval-ordered-basic-test.cpp
    UNIT_TEST(crow_interval_ordered_interval_basic_properties)
    UNIT_TEST(crow_interval_ordered_interval_construction)
    UNIT_TEST(crow_interval_ordered_interval_from_string)
    UNIT_TEST(crow_interval_ordered_interval_order)
    UNIT_TEST(crow_interval_ordered_interval_complement)
    UNIT_TEST(crow_interval_ordered_interval_binary_operations)
    UNIT_TEST(crow_interval_ordered_interval_hashing)

    // interval-ordered-boundary-basic-test.cpp
    UNIT_TEST(crow_interval_ordered_boundary_formatting)
    UNIT_TEST(crow_interval_ordered_boundary_from_interval)
    UNIT_TEST(crow_interval_ordered_boundary_to_interval)

    // interval-ordered-boundary-comparison-test.cpp
    UNIT_TEST(crow_interval_ordered_boundary_adjacency)
    UNIT_TEST(crow_interval_ordered_boundary_comparison)

    // interval-ordered-map-test.cpp
    UNIT_TEST(crow_interval_ordered_interval_map)
    UNIT_TEST(crow_interval_ordered_interval_map_hashing)

    // interval-ordered-set-test.cpp
    UNIT_TEST(crow_interval_ordered_interval_set_construct_insert_erase)
    UNIT_TEST(crow_interval_ordered_interval_set_operations)
    UNIT_TEST(crow_interval_ordered_interval_set_hashing)

    // interval-stepwise-basic-test.cpp
    UNIT_TEST(crow_interval_stepwise_interval_basic_properties)
    UNIT_TEST(crow_interval_stepwise_interval_construction)
    UNIT_TEST(crow_interval_stepwise_interval_from_string)
    UNIT_TEST(crow_interval_stepwise_interval_iterators)
    UNIT_TEST(crow_interval_stepwise_interval_order)
    UNIT_TEST(crow_interval_stepwise_interval_complement)
    UNIT_TEST(crow_interval_stepwise_interval_binary_operations)
    UNIT_TEST(crow_interval_stepwise_interval_hashing)

    // interval-stepwise-boundary-basic-test.cpp
    UNIT_TEST(crow_interval_stepwise_boundary_formatting)
    UNIT_TEST(crow_interval_stepwise_boundary_from_interval)
    UNIT_TEST(crow_interval_stepwise_boundary_to_interval)

    // interval-stepwise-boundary-comparison-test.cpp
    UNIT_TEST(crow_interval_stepwise_boundary_adjacency)
    UNIT_TEST(crow_interval_stepwise_boundary_comparison)

    // interval-stepwise-map-test.cpp
    UNIT_TEST(crow_interval_stepwise_interval_map)
    UNIT_TEST(crow_interval_stepwise_interval_map_hashing)

    // interval-stepwise-set-test.cpp
    UNIT_TEST(crow_interval_stepwise_interval_set_construct_insert_erase)
    UNIT_TEST(crow_interval_stepwise_interval_set_operations)
    UNIT_TEST(crow_interval_stepwise_interval_set_hashing)

    // interval-types-test.cpp
    UNIT_TEST(crow_interval_type_traits)
    UNIT_TEST(crow_interval_string_conversion)

    // iterator-test.cpp
    UNIT_TEST(crow_iterator_mixins)
    UNIT_TEST(crow_iterator_append_overwrite)
    UNIT_TEST(crow_iterator_dereference)
    UNIT_TEST(crow_iterator_iota)
    UNIT_TEST(crow_iterator_subrange)

    // linear-map-test.cpp
    UNIT_TEST(crow_linear_map)

    // markov-test.cpp
    UNIT_TEST(crow_markov_character_mode)
    UNIT_TEST(crow_markov_string_mode)

    // markup-test.cpp
    UNIT_TEST(crow_markup_xml)
    UNIT_TEST(crow_markup_html)

    // maths-test.cpp
    UNIT_TEST(crow_maths_rounding)
    UNIT_TEST(crow_maths_euclidean_division)
    UNIT_TEST(crow_maths_fraction)
    UNIT_TEST(crow_maths_interpolate)
    UNIT_TEST(crow_maths_symmetric_division)
    UNIT_TEST(crow_maths_numeric_literals)

    // matrix-test.cpp
    UNIT_TEST(crow_matrix_basics)
    UNIT_TEST(crow_matrix_inversion)

    // meta-test.cpp
    UNIT_TEST(crow_meta_append)
    UNIT_TEST(crow_meta_concat)
    UNIT_TEST(crow_meta_insert)
    UNIT_TEST(crow_meta_insert_at)
    UNIT_TEST(crow_meta_prefix)
    UNIT_TEST(crow_meta_repeat)
    UNIT_TEST(crow_meta_repeat_list)
    UNIT_TEST(crow_meta_resize)
    UNIT_TEST(crow_meta_skip)
    UNIT_TEST(crow_meta_sublist)
    UNIT_TEST(crow_meta_take)
    UNIT_TEST(crow_meta_at_index)
    UNIT_TEST(crow_meta_head)
    UNIT_TEST(crow_meta_tail)
    UNIT_TEST(crow_meta_most)
    UNIT_TEST(crow_meta_last)
    UNIT_TEST(crow_meta_max_min)
    UNIT_TEST(crow_meta_fold)
    UNIT_TEST(crow_meta_make_set)
    UNIT_TEST(crow_meta_map)
    UNIT_TEST(crow_meta_partial_reduce)
    UNIT_TEST(crow_meta_remove)
    UNIT_TEST(crow_meta_reverse)
    UNIT_TEST(crow_meta_select)
    UNIT_TEST(crow_meta_sort)
    UNIT_TEST(crow_meta_unique)
    UNIT_TEST(crow_meta_zip)
    UNIT_TEST(crow_meta_inherit)
    UNIT_TEST(crow_meta_tuples)
    UNIT_TEST(crow_meta_all_of)
    UNIT_TEST(crow_meta_count)
    UNIT_TEST(crow_meta_find)
    UNIT_TEST(crow_meta_in_list)
    UNIT_TEST(crow_meta_is_empty)
    UNIT_TEST(crow_meta_is_unique)
    UNIT_TEST(crow_meta_length_of)

    // mirror-map-test.cpp
    UNIT_TEST(crow_mirror_map_construct)
    UNIT_TEST(crow_mirror_map_iterators)
    UNIT_TEST(crow_mirror_map_insert)
    UNIT_TEST(crow_mirror_map_erase)
    UNIT_TEST(crow_mirror_map_search)
    UNIT_TEST(crow_mirror_map_duplicates)

    // mp-integer-rational-arithmetic-test.cpp
    UNIT_TEST(crow_mp_integer_rational_reduction)
    UNIT_TEST(crow_mp_integer_rational_arithmetic)
    UNIT_TEST(crow_mp_integer_rational_properties)
    UNIT_TEST(crow_mp_integer_rational_comparison)
    UNIT_TEST(crow_mp_integer_rational_mixed)

    // mp-integer-rational-conversion-test.cpp
    UNIT_TEST(crow_mp_integer_rational_basics)
    UNIT_TEST(crow_mp_integer_rational_parsing)

    // mp-integer-sign-agnostic-test.cpp
    UNIT_TEST(crow_mp_integer_literals)
    UNIT_TEST(crow_mp_integer_core_functions)
    UNIT_TEST(crow_mp_integer_hash_set)

    // mp-integer-signed-arithmetic-test.cpp
    UNIT_TEST(crow_mp_integer_signed_arithmetic)
    UNIT_TEST(crow_mp_integer_signed_division)
    UNIT_TEST(crow_mp_integer_signed_large_arithmetic)
    UNIT_TEST(crow_mp_integer_signed_powers)

    // mp-integer-signed-conversion-test.cpp
    UNIT_TEST(crow_mp_integer_signed_conversion_integers)
    UNIT_TEST(crow_mp_integer_signed_conversion_strings)
    UNIT_TEST(crow_mp_integer_signed_conversion_base_strings)
    UNIT_TEST(crow_mp_integer_signed_conversion_double)

    // mp-integer-unsigned-arithmetic-test.cpp
    UNIT_TEST(crow_mp_integer_unsigned_arithmetic)
    UNIT_TEST(crow_mp_integer_unsigned_arithmetic_powers)
    UNIT_TEST(crow_mp_integer_unsigned_bit_operations)
    UNIT_TEST(crow_mp_integer_unsigned_byte_operations)

    // mp-integer-unsigned-conversion-test.cpp
    UNIT_TEST(crow_mp_integer_unsigned_conversion_integers)
    UNIT_TEST(crow_mp_integer_unsigned_conversion_strings)
    UNIT_TEST(crow_mp_integer_unsigned_conversion_base_strings)
    UNIT_TEST(crow_mp_integer_unsigned_conversion_double)

    // multi-array-test.cpp
    UNIT_TEST(crow_multi_array_2d)
    UNIT_TEST(crow_multi_array_3d)

    // named-mutex-test.cpp
    UNIT_TEST(crow_named_mutex)

    // net-address-test.cpp
    UNIT_TEST(crow_net_ipv4)
    UNIT_TEST(crow_net_ipv6)
    UNIT_TEST(crow_net_socket_address)
    UNIT_TEST(crow_net_ip_literals)

    // net-dns-test.cpp
    UNIT_TEST(crow_net_dns_query)

    // net-tcp-test.cpp
    UNIT_TEST(crow_net_tcp_client_server)
    UNIT_TEST(crow_net_socket_set)

    // noise-test.cpp
    UNIT_TEST(crow_noise_result_stability)
    UNIT_TEST(crow_noise_multiple_sources)
    UNIT_TEST(crow_noise_statistics)
    UNIT_TEST(crow_noise_sample_renders)

    // options-anonymous-test.cpp
    UNIT_TEST(crow_options_anonymous)

    // options-boolean-test.cpp
    UNIT_TEST(crow_options_boolean)

    // options-container-test.cpp
    UNIT_TEST(crow_options_container)

    // options-enumeration-test.cpp
    UNIT_TEST(crow_options_enumeration)

    // options-help-test.cpp
    UNIT_TEST(crow_options_help)

    // options-mutual-exclusion-test.cpp
    UNIT_TEST(crow_options_mutual_exclusion)

    // options-parsing-test.cpp
    UNIT_TEST(crow_options_parsing)

    // options-pattern-match-test.cpp
    UNIT_TEST(crow_options_pattern_match)

    // options-required-test.cpp
    UNIT_TEST(crow_options_required)

    // options-traits-test.cpp
    UNIT_TEST(crow_options_traits)

    // path-directory-test.cpp
    UNIT_TEST(crow_path_directory_iterators)
    UNIT_TEST(crow_path_current_directory)
    UNIT_TEST(crow_path_deep_search)

    // path-file-system-test.cpp
    UNIT_TEST(crow_path_resolution)
    UNIT_TEST(crow_path_file_system_queries)
    UNIT_TEST(crow_path_file_system_updates)
    UNIT_TEST(crow_path_io)
    UNIT_TEST(crow_path_links)
    UNIT_TEST(crow_path_metadata)

    // path-name-test.cpp
    UNIT_TEST(crow_path_legal_names)
    UNIT_TEST(crow_path_unicode_names)
    UNIT_TEST(crow_path_name_normalization)
    UNIT_TEST(crow_path_name_properties)
    UNIT_TEST(crow_path_name_breakdown)
    UNIT_TEST(crow_path_name_combination)
    UNIT_TEST(crow_path_name_manipulation)
    UNIT_TEST(crow_path_name_comparison)

    // prime-fixed-binary-test.cpp
    UNIT_TEST(crow_prime_iterator_fixed_binary)
    UNIT_TEST(crow_prime_primality_fixed_binary)
    UNIT_TEST(crow_prime_next_prev_prime_fixed_binary)
    UNIT_TEST(crow_prime_factorization_fixed_binary)
    UNIT_TEST(crow_prime_list_primes_fixed_binary)

    // prime-int-factors-test.cpp
    UNIT_TEST(crow_prime_factorization_int)

    // prime-int-next-prev-test.cpp
    UNIT_TEST(crow_prime_next_prev_prime_int)

    // prime-int-test.cpp
    UNIT_TEST(crow_prime_iterator_int)
    UNIT_TEST(crow_prime_primality_int)
    UNIT_TEST(crow_prime_list_primes_int)

    // prime-mp-integer-test.cpp
    UNIT_TEST(crow_prime_iterator_mp_integer)
    UNIT_TEST(crow_prime_primality_mp_integer)
    UNIT_TEST(crow_prime_next_prev_prime_mp_integer)
    UNIT_TEST(crow_prime_factorization_mp_integer)
    UNIT_TEST(crow_prime_list_primes_mp_integer)

    // process-test.cpp
    UNIT_TEST(crow_process_stream)
    UNIT_TEST(crow_process_text)
    UNIT_TEST(crow_process_shell_command)

    // progress-test.cpp
    UNIT_TEST(crow_progress_bar)

    // projection-test.cpp
    UNIT_TEST(crow_projection_polar_coordinate_normalisation)
    UNIT_TEST(crow_projection_azimuthal_equidistant)
    UNIT_TEST(crow_projection_gnomonic)
    UNIT_TEST(crow_projection_lambert_azimuthal)
    UNIT_TEST(crow_projection_orthographic)
    UNIT_TEST(crow_projection_stereographic)
    UNIT_TEST(crow_projection_cylindrical_equidistant)
    UNIT_TEST(crow_projection_gall_peters)
    UNIT_TEST(crow_projection_lambert_cylindrical)
    UNIT_TEST(crow_projection_mercator)
    UNIT_TEST(crow_projection_eckert_iv)
    UNIT_TEST(crow_projection_mollweide)
    UNIT_TEST(crow_projection_sinusoidal)
    UNIT_TEST(crow_projection_interrupted_eckert_iv)
    UNIT_TEST(crow_projection_interrupted_mollweide)
    UNIT_TEST(crow_projection_interrupted_sinusoidal)
    UNIT_TEST(crow_projection_sample_maps)

    // quaternion-test.cpp
    UNIT_TEST(crow_quaternion)

    // random-adapter-test.cpp
    UNIT_TEST(crow_random_constrained_distribution)

    // random-continuous-test.cpp
    UNIT_TEST(crow_random_uniform_real_distribution_properties)
    UNIT_TEST(crow_random_uniform_real_distribution)
    UNIT_TEST(crow_random_log_uniform_distribution)
    UNIT_TEST(crow_random_normal_distribution_properties)
    UNIT_TEST(crow_random_normal_distribution)
    UNIT_TEST(crow_random_log_normal_distribution)

    // random-discrete-test.cpp
    UNIT_TEST(crow_random_bernoulli_distribution)
    UNIT_TEST(crow_random_uniform_integer_distribution)
    UNIT_TEST(crow_random_poisson_distribution)

    // random-lcg-test.cpp
    UNIT_TEST(crow_random_lcg_generators)

    // random-non-arithmetic-test.cpp
    UNIT_TEST(crow_random_choice_distribution)
    UNIT_TEST(crow_random_weighted_distribution)
    UNIT_TEST(crow_random_uuid)

    // random-pcg-test.cpp
    UNIT_TEST(crow_random_pcg64)

    // random-spatial-test.cpp
    UNIT_TEST(crow_random_vectors)
    UNIT_TEST(crow_random_point_in_sphere)
    UNIT_TEST(crow_random_direction)

    // random-xoshiro-test.cpp
    UNIT_TEST(crow_random_splitmix64)
    UNIT_TEST(crow_random_xoshiro256ss)

    // rational-test.cpp
    UNIT_TEST(crow_rational_basics)
    UNIT_TEST(crow_rational_reduction)
    UNIT_TEST(crow_rational_arithmetic)
    UNIT_TEST(crow_rational_properties)
    UNIT_TEST(crow_rational_comparison)
    UNIT_TEST(crow_rational_mixed)
    UNIT_TEST(crow_rational_formatting)
    UNIT_TEST(crow_rational_parsing)
    UNIT_TEST(crow_rational_hash)

    // regex-basic-test.cpp
    UNIT_TEST(crow_regex_pcre_version)
    UNIT_TEST(crow_regex_escape)

    // regex-compile-flags-test.cpp
    UNIT_TEST(crow_regex_compile_flags)

    // regex-grep-test.cpp
    UNIT_TEST(crow_regex_grep)
    UNIT_TEST(crow_regex_split)
    UNIT_TEST(crow_regex_partition)
    UNIT_TEST(crow_regex_tokenize)

    // regex-match-test.cpp
    UNIT_TEST(crow_regex_match)

    // regex-replace-test.cpp
    UNIT_TEST(crow_regex_replace)
    UNIT_TEST(crow_regex_transform)

    // regex-runtime-flags-test.cpp
    UNIT_TEST(crow_regex_runtime_flags)

    // root-finding-test.cpp
    UNIT_TEST(crow_root_finding_newton_raphson)

    // signal-test.cpp
    UNIT_TEST(crow_signal_channel)
    UNIT_TEST(crow_signal_name)

    // stack-test.cpp
    UNIT_TEST(crow_stack)

    // statistics-test.cpp
    UNIT_TEST(crow_statistics_univariate)
    UNIT_TEST(crow_statistics_bivariate)
    UNIT_TEST(crow_statistics_combination)

    // stdio-test.cpp
    UNIT_TEST(crow_stdio_cstdio)
    UNIT_TEST(crow_stdio_fdio)
    UNIT_TEST(crow_stdio_pipe)
    UNIT_TEST(crow_stdio_winio)
    UNIT_TEST(crow_stdio_null_device)
    UNIT_TEST(crow_stdio_anonymous_temporary_file)
    UNIT_TEST(crow_stdio_named_temporary_file)

    // string-casing-test.cpp
    UNIT_TEST(crow_string_case_conversion)

    // string-character-test.cpp
    UNIT_TEST(crow_character_functions)

    // string-formatting-test.cpp
    UNIT_TEST(crow_hex_string)
    UNIT_TEST(crow_string_quote)
    UNIT_TEST(crow_roman_numerals)

    // string-literals-test.cpp
    UNIT_TEST(crow_string_literals)

    // string-manipulation-test.cpp
    UNIT_TEST(crow_common_prefix)
    UNIT_TEST(crow_string_indentation)
    UNIT_TEST(crow_string_join)
    UNIT_TEST(crow_string_padding)
    UNIT_TEST(crow_string_partition)
    UNIT_TEST(crow_string_repeat)
    UNIT_TEST(crow_string_replace)
    UNIT_TEST(crow_string_remove)
    UNIT_TEST(crow_string_split)
    UNIT_TEST(crow_string_split_at)
    UNIT_TEST(crow_string_split_lines)
    UNIT_TEST(crow_string_starts_ends_with)
    UNIT_TEST(crow_string_trim)
    UNIT_TEST(crow_string_unqualify)

    // string-parsing-test.cpp
    UNIT_TEST(crow_string_to_boolean)
    UNIT_TEST(crow_string_to_integer)
    UNIT_TEST(crow_string_to_floating_point)

    // string-type-test.cpp
    UNIT_TEST(crow_string_type_name)

    // string-wrapping-test.cpp
    UNIT_TEST(crow_string_unwrap)
    UNIT_TEST(crow_string_word_wrap)

    // table-test.cpp
    UNIT_TEST(crow_table_layout)
    UNIT_TEST(crow_table_formatting)
    UNIT_TEST(crow_table_piecewise)
    UNIT_TEST(crow_table_no_header)

    // terminal-test.cpp
    UNIT_TEST(crow_terminal_control)

    // text-gen-test.cpp
    UNIT_TEST(crow_text_generation_null)
    UNIT_TEST(crow_text_generation_constant)
    UNIT_TEST(crow_text_generation_number)
    UNIT_TEST(crow_text_generation_select)
    UNIT_TEST(crow_text_generation_weighted)
    UNIT_TEST(crow_text_generation_sequence)
    UNIT_TEST(crow_text_generation_optional)
    UNIT_TEST(crow_text_generation_repeat)
    UNIT_TEST(crow_text_generation_transform)
    UNIT_TEST(crow_text_generation_casing)
    UNIT_TEST(crow_text_generation_literals)

    // thread-pool-test.cpp
    UNIT_TEST(crow_thread_pool_class)
    UNIT_TEST(crow_thread_pool_each)
    UNIT_TEST(crow_thread_pool_timing)

    // thread-test.cpp
    UNIT_TEST(crow_thread)

    // time-test.cpp
    UNIT_TEST(crow_time_make_date)
    UNIT_TEST(crow_time_formatting)
    UNIT_TEST(crow_time_date_formatting)
    UNIT_TEST(crow_time_parsing)
    UNIT_TEST(crow_time_date_parsing)
    UNIT_TEST(crow_time_duration_formatting)
    UNIT_TEST(crow_time_point_formatting)
    UNIT_TEST(crow_time_point_conversion)
    UNIT_TEST(crow_time_system_specific_conversions)
    UNIT_TEST(crow_time_waiter)

    // topological-order-test.cpp
    UNIT_TEST(crow_topological_order)
    UNIT_TEST(crow_topological_order_reverse)

    // transform-test.cpp
    UNIT_TEST(crow_2d_transforms)
    UNIT_TEST(crow_3d_transforms)
    UNIT_TEST(crow_projective_geometry)
    UNIT_TEST(crow_primitive_transforms)
    UNIT_TEST(crow_quaternion_transforms)

    // unicode-test.cpp
    UNIT_TEST(crow_unicode_implementation_details)
    UNIT_TEST(crow_unicode_character_encoding)
    UNIT_TEST(crow_unicode_string_encoding)
    UNIT_TEST(crow_unicode_length)
    UNIT_TEST(crow_unicode_properties)
    UNIT_TEST(crow_unicode_width)

    // uri-general-test.cpp
    UNIT_TEST(crow_uri_assembly)
    UNIT_TEST(crow_uri_modifiers)
    UNIT_TEST(crow_uri_navigation)
    UNIT_TEST(crow_uri_encoding)
    UNIT_TEST(crow_uri_query)

    // uri-schemes-test.cpp
    UNIT_TEST(crow_uri_parse_http)
    UNIT_TEST(crow_uri_parse_file)
    UNIT_TEST(crow_uri_parse_mailto)

    // uuid-test.cpp
    UNIT_TEST(crow_uuid)

    // vector-test.cpp
    UNIT_TEST(crow_integer_vector_construction)
    UNIT_TEST(crow_integer_vector_arithmetic)
    UNIT_TEST(crow_integer_vector_hash)
    UNIT_TEST(crow_integer_vector_interpolation)
    UNIT_TEST(crow_integer_vector_total_order)
    UNIT_TEST(crow_floating_vector_construction)
    UNIT_TEST(crow_floating_vector_arithmetic)
    UNIT_TEST(crow_floating_vector_interpolation)

    // version-test.cpp
    UNIT_TEST(crow_version)

    // unit-test.cpp

    return Crow::UnitTest::end_tests();

}
