// This file is generated by the rs-update-tests script

#include "crow/unit-test.hpp"

void version_test_group() {
    UNIT_TEST(crow_version)
}

void log_test_group() {
    UNIT_TEST(crow_logging)
    UNIT_TEST(crow_logging_output)
}

void types_test_group() {
    UNIT_TEST(crow_types_cloneable)
    UNIT_TEST(crow_types_assertions)
    UNIT_TEST(crow_types_comparison_functions)
    UNIT_TEST(crow_types_general_concepts)
    UNIT_TEST(crow_types_primitive_type_concepts)
    UNIT_TEST(crow_types_iterator_concepts)
    UNIT_TEST(crow_types_range_concepts)
    UNIT_TEST(crow_types_container_concepts)
    UNIT_TEST(crow_types_literals)
    UNIT_TEST(crow_types_function_objects)
}

void algorithm_test_group() {
    UNIT_TEST(crow_algorithm_lookup)
    UNIT_TEST(crow_algorithm_unique)
    UNIT_TEST(crow_algorithm_diff)
    UNIT_TEST(crow_algorithm_edit_distance_levenshtein)
    UNIT_TEST(crow_algorithm_edit_distance_damerau_levenshtein)
    UNIT_TEST(crow_algorithm_edit_distance_jaro_winkler)
    UNIT_TEST(crow_algorithm_hash_compare)
    UNIT_TEST(crow_algorithm_interpolation)
    UNIT_TEST(crow_algorithm_interpolated_map)
    UNIT_TEST(crow_algorithm_interpolated_map_lin_log)
    UNIT_TEST(crow_algorithm_interpolated_map_log_log)
    UNIT_TEST(crow_algorithm_cubic_spline_map)
    UNIT_TEST(crow_algorithm_precision_sum)
    UNIT_TEST(crow_algorithm_line_integral)
    UNIT_TEST(crow_algorithm_volume_integral)
    UNIT_TEST(crow_algorithm_cartesian_power)
    UNIT_TEST(crow_algorithm_find_optimum)
    UNIT_TEST(crow_algorithm_subsets)
}

void approx_arithmetic_test_group() {
    UNIT_TEST(crow_approx_arithmetic)
}

void approx_construction_test_group() {
    UNIT_TEST(crow_approx_construction)
    UNIT_TEST(crow_approx_parsing)
}

void approx_formatting_test_group() {
    UNIT_TEST(crow_approx_formatting)
}

void benchmark_test_group() {
    UNIT_TEST(crow_benchmark)
}

void binary_test_group() {
    UNIT_TEST(crow_binary_bit_manipulation)
    UNIT_TEST(crow_binary_byte_order)
    UNIT_TEST(crow_binary_signed_overflow_detection)
    UNIT_TEST(crow_binary_unsigned_overflow_detection)
}

void bounded_array_construction_test_group() {
    UNIT_TEST(crow_bounded_array_construction)
}

void bounded_array_insertion_test_group() {
    UNIT_TEST(crow_bounded_array_insertion)
}

void bounded_array_misc_test_group() {
    UNIT_TEST(crow_bounded_array_capacity)
    UNIT_TEST(crow_bounded_array_keys)
}

void bounded_array_tracking_test_group() {
    UNIT_TEST(crow_bounded_array_tracking)
}

void cache_test_group() {
    UNIT_TEST(crow_cache_hashed)
    UNIT_TEST(crow_cache_ordered)
    UNIT_TEST(crow_cache_function)
}

void colour_alpha_test_group() {
    UNIT_TEST(crow_colour_concepts)
    UNIT_TEST(crow_colour_channel_order)
    UNIT_TEST(crow_colour_premultiplied_alpha)
    UNIT_TEST(crow_colour_alpha_blending)
}

void colour_conversion_test_group() {
    UNIT_TEST(crow_colour_conversion_between_colour_spaces)
    UNIT_TEST(crow_colour_conversion_between_representations)
}

void colour_floating_channel_test_group() {
    UNIT_TEST(crow_colour_floating_point_elements)
    UNIT_TEST(crow_colour_floating_point_arithmetic)
    UNIT_TEST(crow_colour_floating_point_standard_colours)
}

void colour_integer_channel_test_group() {
    UNIT_TEST(crow_colour_integral_elements)
    UNIT_TEST(crow_colour_integral_standard_colours)
    UNIT_TEST(crow_colour_integral_arithmetic)
}

void colour_interpolation_test_group() {
    UNIT_TEST(crow_colour_interpolation)
}

void colour_space_test_group() {
    UNIT_TEST(crow_colour_space_concepts)
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
}

void colour_string_test_group() {
    UNIT_TEST(crow_colour_hex_representation)
    UNIT_TEST(crow_colour_css_colours)
}

void compact_array_construction_test_group() {
    UNIT_TEST(crow_compact_array_construction)
}

void compact_array_insertion_test_group() {
    UNIT_TEST(crow_compact_array_insertion)
}

void compact_array_misc_test_group() {
    UNIT_TEST(crow_compact_array_capacity)
    UNIT_TEST(crow_compact_array_keys)
}

void compact_array_tracking_test_group() {
    UNIT_TEST(crow_compact_array_tracking)
}

void constants_test_group() {
    UNIT_TEST(crow_constants)
}

void dice_test_group() {
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
}

void dso_test_group() {
    UNIT_TEST(crow_dso_loading)
}

void encoding_test_group() {
    UNIT_TEST(crow_encoding_hexcode)
    UNIT_TEST(crow_encoding_base64)
}

void english_test_group() {
    UNIT_TEST(crow_english_extended_titlecase)
    UNIT_TEST(crow_english_sentence_case)
    UNIT_TEST(crow_english_list_formatting)
    UNIT_TEST(crow_english_number_formatting)
    UNIT_TEST(crow_english_pluralization)
    UNIT_TEST(crow_english_lorem_ipsum)
}

void enum_test_group() {
    UNIT_TEST(crow_format_enum_definition)
    UNIT_TEST(crow_format_enum_bitmask_operators)
}

void fixed_binary_large_binary_100_test_group() {
    UNIT_TEST(crow_fixed_binary_large_binary_100)
}

void fixed_binary_large_binary_35_test_group() {
    UNIT_TEST(crow_fixed_binary_large_binary_35)
}

void fixed_binary_misc_test_group() {
    UNIT_TEST(crow_fixed_binary_implementation_selection)
    UNIT_TEST(crow_fixed_binary_type_conversions)
    UNIT_TEST(crow_fixed_binary_string_parsing)
    UNIT_TEST(crow_fixed_binary_hash_set)
}

void fixed_binary_small_binary_35_test_group() {
    UNIT_TEST(crow_fixed_binary_small_binary_35)
}

void fixed_binary_small_binary_5_test_group() {
    UNIT_TEST(crow_fixed_binary_small_binary_5)
}

void flexible_map_test_group() {
    UNIT_TEST(crow_flexible_map_order)
    UNIT_TEST(crow_flexible_map_hash)
    UNIT_TEST(crow_flexible_map_linear)
}

void flexible_set_test_group() {
    UNIT_TEST(crow_flexible_set_order)
    UNIT_TEST(crow_flexible_set_hash)
    UNIT_TEST(crow_flexible_set_linear)
}

void font_test_group() {
    UNIT_TEST(crow_font_loading)
    UNIT_TEST(crow_font_properties)
    UNIT_TEST(crow_font_metrics)
    UNIT_TEST(crow_font_text_metrics)
    UNIT_TEST(crow_font_text_fitting)
    UNIT_TEST(crow_font_text_wrapping)
    UNIT_TEST(crow_font_rendering)
    UNIT_TEST(crow_font_map)
}

void format_floating_test_group() {
    UNIT_TEST(crow_format_floating_point_significant_digits_format)
    UNIT_TEST(crow_format_floating_point_scientific_notation)
    UNIT_TEST(crow_format_floating_point_fixed_point_format)
    UNIT_TEST(crow_format_floating_point_general_format)
    UNIT_TEST(crow_format_floating_point_probability_format)
}

void format_integer_test_group() {
    UNIT_TEST(crow_format_signed_integers)
    UNIT_TEST(crow_format_unsigned_integers)
}

void format_misc_test_group() {
    UNIT_TEST(crow_format_null_values)
    UNIT_TEST(crow_format_std_ordering)
    UNIT_TEST(crow_format_std_optional)
}

void format_numeric_test_group() {
    UNIT_TEST(crow_format_integer_delimiters)
    UNIT_TEST(crow_format_floating_point_delimiters)
}

void format_print_test_group() {
    UNIT_TEST(crow_format_print)
}

void format_range_test_group() {
    UNIT_TEST(crow_format_string)
    UNIT_TEST(crow_format_ranges)
}

void format_type_test_group() {
    UNIT_TEST(crow_format_concepts)
    UNIT_TEST(crow_format_custom_types)
    UNIT_TEST(crow_format_boolean)
    UNIT_TEST(crow_format_characters_and_strings)
    UNIT_TEST(crow_format_pointers)
    UNIT_TEST(crow_format_type_names)
    UNIT_TEST(crow_format_object)
    UNIT_TEST(crow_format_class)
    UNIT_TEST(crow_format_function)
    UNIT_TEST(crow_format_literals)
}

void formula_test_group() {
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
}

void geometry_test_group() {
    UNIT_TEST(crow_geometry_box)
    UNIT_TEST(crow_geometry_sphere)
}

void guard_test_group() {
    UNIT_TEST(crow_scope_guard)
    UNIT_TEST(crow_scope_guard_multiple_action)
}

void hash_test_group() {
    UNIT_TEST(crow_hash_concepts)
    UNIT_TEST(crow_hash_mix)
    UNIT_TEST(crow_hash_multiplicative)
    UNIT_TEST(crow_hash_siphash)
    UNIT_TEST(crow_hash_md5)
    UNIT_TEST(crow_hash_sha1)
    UNIT_TEST(crow_hash_sha256)
    UNIT_TEST(crow_hash_sha512)
}

void hexmap_art_test_group() {
    UNIT_TEST(crow_hexmap_ascii_art)
    UNIT_TEST(crow_hexmap_emoji_art)
    UNIT_TEST(crow_hexmap_xterm_art)
}

void hexmap_building_test_group() {
    UNIT_TEST(crow_hexmap_adjacency_properties)
    UNIT_TEST(crow_hexmap_initializer_list)
}

void hexmap_geometry_test_group() {
    UNIT_TEST(crow_hexmap_coordinate_operations)
}

void image_io_test_group() {
    UNIT_TEST(crow_image_io_file_info)
    UNIT_TEST(crow_image_io_load)
    UNIT_TEST(crow_image_io_save)
}

void image_resize_test_group() {
    UNIT_TEST(crow_image_resize_dimensions)
    UNIT_TEST(crow_image_resize_content)
    UNIT_TEST(crow_image_segment)
}

void image_test_group() {
    UNIT_TEST(crow_image_construction)
    UNIT_TEST(crow_image_pixel_access)
    UNIT_TEST(crow_image_premultiplied_alpha)
    UNIT_TEST(crow_image_conversion)
}

void index_table_test_group() {
    UNIT_TEST(crow_index_table_classes)
}

void iterator_test_group() {
    UNIT_TEST(crow_iterator_mixins)
    UNIT_TEST(crow_iterator_append_overwrite)
    UNIT_TEST(crow_iterator_dereference)
    UNIT_TEST(crow_iterator_iota)
    UNIT_TEST(crow_iterator_subrange)
}

void linear_map_test_group() {
    UNIT_TEST(crow_linear_map)
}

void log_scale_arithmetic_test_group() {
    UNIT_TEST(crow_log_scale_arithmetic)
    UNIT_TEST(crow_log_scale_comparison)
    UNIT_TEST(crow_log_scale_elementary_functions)
}

void log_scale_basics_test_group() {
    UNIT_TEST(crow_log_scale_properties)
    UNIT_TEST(crow_log_scale_construction_from_value)
    UNIT_TEST(crow_log_scale_construction_from_log_and_sign)
    UNIT_TEST(crow_log_scale_range)
    UNIT_TEST(crow_log_scale_hash)
}

void log_scale_formatting_test_group() {
    UNIT_TEST(crow_log_scale_format_default)
    UNIT_TEST(crow_log_scale_format_digits)
    UNIT_TEST(crow_log_scale_format_exponential)
    UNIT_TEST(crow_log_scale_format_general)
    UNIT_TEST(crow_log_scale_format_extreme_values)
    UNIT_TEST(crow_log_scale_format_zeroes)
}

void log_scale_nested_test_group() {
    UNIT_TEST(crow_log_scale_nested_construction)
}

void log_scale_parsing_test_group() {
    UNIT_TEST(crow_log_scale_parsing)
    UNIT_TEST(crow_log_scale_parsing_extreme_values)
}

void markov_test_group() {
    UNIT_TEST(crow_markov_character_mode)
    UNIT_TEST(crow_markov_string_mode)
}

void markup_test_group() {
    UNIT_TEST(crow_markup_xml)
    UNIT_TEST(crow_markup_html)
}

void maths_arithmetic_functions_test_group() {
    UNIT_TEST(crow_maths_binomial_coefficients)
    UNIT_TEST(crow_maths_euclidean_division)
    UNIT_TEST(crow_maths_symmetric_division)
    UNIT_TEST(crow_maths_integer_power)
    UNIT_TEST(crow_maths_integer_power_generalised)
    UNIT_TEST(crow_maths_interpolation)
}

void maths_literals_test_group() {
    UNIT_TEST(crow_maths_numeric_literals)
}

void maths_properties_test_group() {
    UNIT_TEST(crow_maths_rounding)
    UNIT_TEST(crow_maths_emodf)
    UNIT_TEST(crow_maths_sign)
}

void maths_special_functions_test_group() {
    UNIT_TEST(crow_maths_inverse_erf)
}

void matrix_test_group() {
    UNIT_TEST(crow_matrix_basics)
    UNIT_TEST(crow_matrix_inversion)
}

void meta_test_group() {
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
}

void mirror_map_test_group() {
    UNIT_TEST(crow_mirror_map_construct)
    UNIT_TEST(crow_mirror_map_iterators)
    UNIT_TEST(crow_mirror_map_insert)
    UNIT_TEST(crow_mirror_map_erase)
    UNIT_TEST(crow_mirror_map_search)
    UNIT_TEST(crow_mirror_map_duplicates)
}

void mp_integer_rational_arithmetic_test_group() {
    UNIT_TEST(crow_mp_integer_rational_reduction)
    UNIT_TEST(crow_mp_integer_rational_arithmetic)
    UNIT_TEST(crow_mp_integer_rational_properties)
    UNIT_TEST(crow_mp_integer_rational_comparison)
    UNIT_TEST(crow_mp_integer_rational_mixed)
}

void mp_integer_rational_conversion_test_group() {
    UNIT_TEST(crow_mp_integer_rational_basics)
    UNIT_TEST(crow_mp_integer_rational_parsing)
}

void mp_integer_sign_agnostic_test_group() {
    UNIT_TEST(crow_mp_integer_literals)
    UNIT_TEST(crow_mp_integer_core_functions)
    UNIT_TEST(crow_mp_integer_hash_set)
}

void mp_integer_signed_arithmetic_test_group() {
    UNIT_TEST(crow_mp_integer_signed_arithmetic)
    UNIT_TEST(crow_mp_integer_signed_division)
    UNIT_TEST(crow_mp_integer_signed_large_arithmetic)
    UNIT_TEST(crow_mp_integer_signed_powers)
}

void mp_integer_signed_conversion_test_group() {
    UNIT_TEST(crow_mp_integer_signed_conversion_integers)
    UNIT_TEST(crow_mp_integer_signed_conversion_strings)
    UNIT_TEST(crow_mp_integer_signed_conversion_base_strings)
    UNIT_TEST(crow_mp_integer_signed_conversion_double)
}

void mp_integer_unsigned_arithmetic_test_group() {
    UNIT_TEST(crow_mp_integer_unsigned_arithmetic)
    UNIT_TEST(crow_mp_integer_unsigned_arithmetic_powers)
    UNIT_TEST(crow_mp_integer_unsigned_bit_operations)
    UNIT_TEST(crow_mp_integer_unsigned_byte_operations)
}

void mp_integer_unsigned_conversion_test_group() {
    UNIT_TEST(crow_mp_integer_unsigned_conversion_integers)
    UNIT_TEST(crow_mp_integer_unsigned_conversion_strings)
    UNIT_TEST(crow_mp_integer_unsigned_conversion_base_strings)
    UNIT_TEST(crow_mp_integer_unsigned_conversion_double)
}

void multi_array_test_group() {
    UNIT_TEST(crow_multi_array_2d)
    UNIT_TEST(crow_multi_array_3d)
    UNIT_TEST(crow_multi_array_bounded)
    UNIT_TEST(crow_multi_array_compact)
}

void name_test_group() {
    UNIT_TEST(crow_name_parsing)
    UNIT_TEST(crow_name_formatting)
    UNIT_TEST(crow_name_from_object)
}

void named_mutex_test_group() {
    UNIT_TEST(crow_named_mutex)
}

void noise_test_group() {
    UNIT_TEST(crow_noise_result_stability)
    UNIT_TEST(crow_noise_multiple_sources)
    UNIT_TEST(crow_noise_statistics)
    UNIT_TEST(crow_noise_sample_renders)
}

void options_anonymous_test_group() {
    UNIT_TEST(crow_options_anonymous)
}

void options_boolean_test_group() {
    UNIT_TEST(crow_options_boolean)
}

void options_container_test_group() {
    UNIT_TEST(crow_options_container)
    UNIT_TEST(crow_options_container_defaults)
}

void options_enumeration_test_group() {
    UNIT_TEST(crow_options_enumeration)
}

void options_help_test_group() {
    UNIT_TEST(crow_options_help)
}

void options_mutual_exclusion_test_group() {
    UNIT_TEST(crow_options_mutual_exclusion)
}

void options_parsing_test_group() {
    UNIT_TEST(crow_options_parsing)
}

void options_path_test_group() {
    UNIT_TEST(crow_options_path)
}

void options_pattern_match_test_group() {
    UNIT_TEST(crow_options_pattern_match)
}

void options_required_test_group() {
    UNIT_TEST(crow_options_required)
}

void options_traits_test_group() {
    UNIT_TEST(crow_options_traits_concepts)
}

void options_types_test_group() {
    UNIT_TEST(crow_options_types)
}

void path_directory_test_group() {
    UNIT_TEST(crow_path_directory_iterators)
    UNIT_TEST(crow_path_current_directory)
    UNIT_TEST(crow_path_current_directory_contents)
    UNIT_TEST(crow_path_deep_search)
}

void path_file_system_test_group() {
    UNIT_TEST(crow_path_resolution)
    UNIT_TEST(crow_path_file_system_queries)
    UNIT_TEST(crow_path_file_system_updates)
    UNIT_TEST(crow_path_io)
    UNIT_TEST(crow_path_links)
    UNIT_TEST(crow_path_metadata)
}

void path_name_test_group() {
    UNIT_TEST(crow_path_legal_names)
    UNIT_TEST(crow_path_unicode_names)
    UNIT_TEST(crow_path_name_normalization)
    UNIT_TEST(crow_path_name_properties)
    UNIT_TEST(crow_path_name_breakdown)
    UNIT_TEST(crow_path_name_combination)
    UNIT_TEST(crow_path_name_manipulation)
    UNIT_TEST(crow_path_name_comparison)
}

void prime_fixed_binary_test_group() {
    UNIT_TEST(crow_prime_iterator_fixed_binary)
    UNIT_TEST(crow_prime_primality_fixed_binary)
    UNIT_TEST(crow_prime_next_prev_prime_fixed_binary)
    UNIT_TEST(crow_prime_factorization_fixed_binary)
    UNIT_TEST(crow_prime_list_primes_fixed_binary)
}

void prime_int_factors_test_group() {
    UNIT_TEST(crow_prime_factorization_int)
}

void prime_int_next_prev_test_group() {
    UNIT_TEST(crow_prime_next_prev_prime_int)
}

void prime_int_test_group() {
    UNIT_TEST(crow_prime_iterator_int)
    UNIT_TEST(crow_prime_primality_int)
    UNIT_TEST(crow_prime_list_primes_int)
}

void prime_mp_integer_test_group() {
    UNIT_TEST(crow_prime_iterator_mp_integer)
    UNIT_TEST(crow_prime_primality_mp_integer)
    UNIT_TEST(crow_prime_next_prev_prime_mp_integer)
    UNIT_TEST(crow_prime_factorization_mp_integer)
    UNIT_TEST(crow_prime_list_primes_mp_integer)
}

void probability_arithmetic_test_group() {
    UNIT_TEST(crow_probability_complement)
    UNIT_TEST(crow_probability_addition_subtraction)
    UNIT_TEST(crow_probability_multiplication_division)
}

void probability_special_functions_test_group() {
    UNIT_TEST(crow_probability_powers)
    UNIT_TEST(crow_probability_normal_variates)
}

void probability_values_test_group() {
    UNIT_TEST(crow_probability_construction)
    UNIT_TEST(crow_probability_formatting)
    UNIT_TEST(crow_probability_comparison)
}

void progress_test_group() {
    UNIT_TEST(crow_progress_bar)
}

void projection_test_group() {
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
}

void quaternion_test_group() {
    UNIT_TEST(crow_quaternion)
}

void random_adapter_test_group() {
    UNIT_TEST(crow_random_constrained_distribution)
}

void random_algorithm_test_group() {
    UNIT_TEST(crow_random_sample)
    UNIT_TEST(crow_random_shuffle)
}

void random_concept_test_group() {
    UNIT_TEST(crow_random_concepts)
}

void random_continuous_test_group() {
    UNIT_TEST(crow_random_uniform_real_distribution_properties)
    UNIT_TEST(crow_random_uniform_real_distribution)
    UNIT_TEST(crow_random_log_uniform_distribution)
    UNIT_TEST(crow_random_normal_distribution_properties)
    UNIT_TEST(crow_random_normal_distribution)
    UNIT_TEST(crow_random_log_normal_distribution)
}

void random_discrete_test_group() {
    UNIT_TEST(crow_random_bernoulli_distribution)
    UNIT_TEST(crow_random_uniform_integer_distribution)
    UNIT_TEST(crow_random_poisson_distribution)
}

void random_lcg_test_group() {
    UNIT_TEST(crow_random_lcg_32)
    UNIT_TEST(crow_random_lcg_64)
    UNIT_TEST(crow_random_lcg_128)
}

void random_non_arithmetic_test_group() {
    UNIT_TEST(crow_random_choice_distribution)
    UNIT_TEST(crow_random_weighted_distribution)
    UNIT_TEST(crow_random_uuid)
}

void random_pcg_test_group() {
    UNIT_TEST(crow_random_pcg64)
}

void random_seed_test_group() {
    UNIT_TEST(crow_random_seed_from_device)
}

void random_spatial_test_group() {
    UNIT_TEST(crow_random_vectors)
    UNIT_TEST(crow_random_point_in_sphere)
    UNIT_TEST(crow_random_direction)
}

void random_squirrel_test_group() {
    UNIT_TEST(crow_random_squirrel_32)
    UNIT_TEST(crow_random_squirrel_64)
}

void random_xoshiro_test_group() {
    UNIT_TEST(crow_random_splitmix64)
    UNIT_TEST(crow_random_xoshiro256ss)
}

void rational_test_group() {
    UNIT_TEST(crow_rational_basics)
    UNIT_TEST(crow_rational_reduction)
    UNIT_TEST(crow_rational_arithmetic)
    UNIT_TEST(crow_rational_properties)
    UNIT_TEST(crow_rational_comparison)
    UNIT_TEST(crow_rational_mixed)
    UNIT_TEST(crow_rational_formatting)
    UNIT_TEST(crow_rational_parsing)
    UNIT_TEST(crow_rational_hash)
}

void regex_basic_test_group() {
    UNIT_TEST(crow_regex_pcre_version)
    UNIT_TEST(crow_regex_escape)
}

void regex_compile_flags_test_group() {
    UNIT_TEST(crow_regex_compile_flags)
}

void regex_grep_test_group() {
    UNIT_TEST(crow_regex_grep)
    UNIT_TEST(crow_regex_split)
    UNIT_TEST(crow_regex_partition)
    UNIT_TEST(crow_regex_tokenize)
}

void regex_match_test_group() {
    UNIT_TEST(crow_regex_match)
}

void regex_replace_test_group() {
    UNIT_TEST(crow_regex_replace)
    UNIT_TEST(crow_regex_transform)
}

void regex_runtime_flags_test_group() {
    UNIT_TEST(crow_regex_runtime_flags)
}

void resource_test_group() {
    UNIT_TEST(crow_resource_handle)
}

void root_finding_test_group() {
    UNIT_TEST(crow_root_finding_bisection)
    UNIT_TEST(crow_root_finding_false_position)
    UNIT_TEST(crow_root_finding_newton_raphson)
}

void spatial_index_test_group() {
    UNIT_TEST(crow_spatial_index_construction)
    UNIT_TEST(crow_spatial_index_insert)
    UNIT_TEST(crow_spatial_index_find)
    UNIT_TEST(crow_spatial_index_erase)
    UNIT_TEST(crow_spatial_index_search)
}

void spectrum_formatting_test_group() {
    UNIT_TEST(crow_spectrum_enumerations)
    UNIT_TEST(crow_spectrum_formatting)
}

void spectrum_parsing_test_group() {
    UNIT_TEST(crow_spectrum_parsing)
}

void spectrum_property_test_group() {
    UNIT_TEST(crow_spectrum_basic_properties)
    UNIT_TEST(crow_spectrum_physical_properties)
    UNIT_TEST(crow_spectrum_inverse_properties)
}

void sqlite_test_group() {
    UNIT_TEST(crow_sqlite_connection)
}

void stack_test_group() {
    UNIT_TEST(crow_stack)
}

void statistics_test_group() {
    UNIT_TEST(crow_statistics_univariate)
    UNIT_TEST(crow_statistics_bivariate)
    UNIT_TEST(crow_statistics_combination)
}

void stdio_test_group() {
    UNIT_TEST(crow_stdio_cstdio)
    UNIT_TEST(crow_stdio_fdio)
    UNIT_TEST(crow_stdio_pipe)
    UNIT_TEST(crow_stdio_winio)
    UNIT_TEST(crow_stdio_null_device)
    UNIT_TEST(crow_stdio_anonymous_temporary_file)
    UNIT_TEST(crow_stdio_named_temporary_file)
}

void string_casing_test_group() {
    UNIT_TEST(crow_string_case_conversion)
}

void string_character_test_group() {
    UNIT_TEST(crow_character_functions)
}

void string_formatting_test_group() {
    UNIT_TEST(crow_string_formatting_bin)
    UNIT_TEST(crow_string_formatting_dec)
    UNIT_TEST(crow_string_formatting_hex)
    UNIT_TEST(crow_string_formatting_quote)
    UNIT_TEST(crow_string_formatting_roman)
}

void string_literals_test_group() {
    UNIT_TEST(crow_string_literals)
}

void string_manipulation_test_group() {
    UNIT_TEST(crow_string_comparison)
    UNIT_TEST(crow_string_common_prefix)
    UNIT_TEST(crow_string_concatenation)
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
    UNIT_TEST(crow_string_trim)
    UNIT_TEST(crow_string_unqualify)
}

void string_parsing_test_group() {
    UNIT_TEST(crow_string_to_boolean)
    UNIT_TEST(crow_string_to_integer)
    UNIT_TEST(crow_string_to_floating_point)
}

void string_query_test_group() {
    UNIT_TEST(crow_string_line_and_column)
}

void string_type_test_group() {
    UNIT_TEST(crow_string_type_name)
}

void string_view_test_group() {
    UNIT_TEST(crow_string_view_null)
    UNIT_TEST(crow_string_view_is_substring)
    UNIT_TEST(crow_string_view_position)
    UNIT_TEST(crow_string_view_begin_end)
    UNIT_TEST(crow_string_view_cat)
    UNIT_TEST(crow_string_view_extend)
    UNIT_TEST(crow_string_view_left_right)
    UNIT_TEST(crow_string_view_trim)
}

void string_wrapping_test_group() {
    UNIT_TEST(crow_string_unwrap)
    UNIT_TEST(crow_string_word_wrap)
}

void superposition_test_group() {
    UNIT_TEST(superposition_flat_construction)
    UNIT_TEST(superposition_flat_arithmetic)
    UNIT_TEST(superposition_flat_combination)
    UNIT_TEST(superposition_weighted_construction)
    UNIT_TEST(superposition_weighted_arithmetic)
    UNIT_TEST(superposition_weighted_combination)
}

void table_test_group() {
    UNIT_TEST(crow_table_layout)
    UNIT_TEST(crow_table_formatting)
    UNIT_TEST(crow_table_piecewise)
}

void terminal_test_group() {
    UNIT_TEST(crow_terminal_control)
}

void text_gen_test_group() {
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
}

void thread_pool_test_group() {
    UNIT_TEST(crow_thread_pool_class)
    UNIT_TEST(crow_thread_pool_each)
    UNIT_TEST(crow_thread_pool_timing)
}

void thread_test_group() {
    UNIT_TEST(crow_thread)
}

void time_test_group() {
    UNIT_TEST(crow_time_make_date)
    UNIT_TEST(crow_time_formatting)
    UNIT_TEST(crow_time_date_formatting)
    UNIT_TEST(crow_time_parsing)
    UNIT_TEST(crow_time_date_parsing)
    UNIT_TEST(crow_time_point_formatting)
    UNIT_TEST(crow_time_point_conversion)
    UNIT_TEST(crow_time_system_specific_conversions)
    UNIT_TEST(crow_time_waiter)
}

void topological_order_test_group() {
    UNIT_TEST(crow_topological_order)
    UNIT_TEST(crow_topological_order_reverse)
}

void transform_test_group() {
    UNIT_TEST(crow_2d_transforms)
    UNIT_TEST(crow_3d_transforms)
    UNIT_TEST(crow_projective_geometry)
    UNIT_TEST(crow_primitive_transforms)
    UNIT_TEST(crow_quaternion_transforms)
}

void unicode_normalization_test_group() {
    UNIT_TEST(crow_unicode_normalization_forms)
    UNIT_TEST(crow_unicode_normalization_mixed)
}

void unicode_test_group() {
    UNIT_TEST(crow_unicode_implementation_details)
    UNIT_TEST(crow_unicode_properties)
    UNIT_TEST(crow_unicode_character_encoding)
    UNIT_TEST(crow_unicode_string_encoding)
    UNIT_TEST(crow_unicode_utf_iterators)
    UNIT_TEST(crow_unicode_grapheme_iterators)
    UNIT_TEST(crow_unicode_size_units)
    UNIT_TEST(crow_unicode_size_scalars)
    UNIT_TEST(crow_unicode_size_graphemes)
    UNIT_TEST(crow_unicode_size_columns)
    UNIT_TEST(crow_unicode_canonical_combining_class)
    UNIT_TEST(crow_unicode_canonical_composition)
    UNIT_TEST(crow_unicode_canonical_decomposition)
    UNIT_TEST(crow_unicode_hangul_syllable_type)
    UNIT_TEST(crow_unicode_noncharacters)
    UNIT_TEST(crow_unicode_pattern_syntax)
    UNIT_TEST(crow_unicode_xid_properties)
}

void uri_general_test_group() {
    UNIT_TEST(crow_uri_assembly)
    UNIT_TEST(crow_uri_modifiers)
    UNIT_TEST(crow_uri_navigation)
    UNIT_TEST(crow_uri_encoding)
    UNIT_TEST(crow_uri_query)
}

void uri_schemes_test_group() {
    UNIT_TEST(crow_uri_parse_http)
    UNIT_TEST(crow_uri_parse_file)
    UNIT_TEST(crow_uri_parse_mailto)
}

void uuid_test_group() {
    UNIT_TEST(crow_uuid)
}

void vector_test_group() {
    UNIT_TEST(crow_integer_vector_construction)
    UNIT_TEST(crow_integer_vector_arithmetic)
    UNIT_TEST(crow_integer_vector_hash)
    UNIT_TEST(crow_integer_vector_interpolation)
    UNIT_TEST(crow_integer_vector_total_order)
    UNIT_TEST(crow_floating_vector_construction)
    UNIT_TEST(crow_floating_vector_arithmetic)
    UNIT_TEST(crow_floating_vector_interpolation)
}

void web_client_test_group() {
    UNIT_TEST(crow_web_client_http_get)
    UNIT_TEST(crow_web_client_http_head)
}

void xml_construction_test_group() {
    UNIT_TEST(crow_xml_construct_simple_nodes)
    UNIT_TEST(crow_xml_construct_element)
    UNIT_TEST(crow_xml_construct_document)
}

void xml_functions_test_group() {
    UNIT_TEST(crow_xml_functions_character_encoding)
    UNIT_TEST(crow_xml_functions_character_decoding)
    UNIT_TEST(crow_xml_functions_name_validation)
}

void xml_options_test_group() {
    UNIT_TEST(crow_xml_options_autoclose)
    UNIT_TEST(crow_xml_options_foldws)
    UNIT_TEST(crow_xml_options_icase)
    UNIT_TEST(crow_xml_options_keyonly)
    UNIT_TEST(crow_xml_options_noxmldecl)
    UNIT_TEST(crow_xml_options_selfclose)
    UNIT_TEST(crow_xml_options_xentity)
}

void xml_parsing_test_group() {
    UNIT_TEST(crow_xml_parse_cdata)
    UNIT_TEST(crow_xml_parse_comment)
    UNIT_TEST(crow_xml_parse_entity)
    UNIT_TEST(crow_xml_parse_processing)
    UNIT_TEST(crow_xml_parse_text)
    UNIT_TEST(crow_xml_parse_text_entities)
    UNIT_TEST(crow_xml_parse_prologueext)
    UNIT_TEST(crow_xml_parse_simple_elements)
    UNIT_TEST(crow_xml_parse_complex_elements)
}

int main(int argc, char** argv) {

    Crow::UnitTest::begin_tests(argc, argv);

    version_test_group();
    log_test_group();
    types_test_group();
    algorithm_test_group();
    approx_arithmetic_test_group();
    approx_construction_test_group();
    approx_formatting_test_group();
    benchmark_test_group();
    binary_test_group();
    bounded_array_construction_test_group();
    bounded_array_insertion_test_group();
    bounded_array_misc_test_group();
    bounded_array_tracking_test_group();
    cache_test_group();
    colour_alpha_test_group();
    colour_conversion_test_group();
    colour_floating_channel_test_group();
    colour_integer_channel_test_group();
    colour_interpolation_test_group();
    colour_space_test_group();
    colour_string_test_group();
    compact_array_construction_test_group();
    compact_array_insertion_test_group();
    compact_array_misc_test_group();
    compact_array_tracking_test_group();
    constants_test_group();
    dice_test_group();
    dso_test_group();
    encoding_test_group();
    english_test_group();
    enum_test_group();
    fixed_binary_large_binary_100_test_group();
    fixed_binary_large_binary_35_test_group();
    fixed_binary_misc_test_group();
    fixed_binary_small_binary_35_test_group();
    fixed_binary_small_binary_5_test_group();
    flexible_map_test_group();
    flexible_set_test_group();
    font_test_group();
    format_floating_test_group();
    format_integer_test_group();
    format_misc_test_group();
    format_numeric_test_group();
    format_print_test_group();
    format_range_test_group();
    format_type_test_group();
    formula_test_group();
    geometry_test_group();
    guard_test_group();
    hash_test_group();
    hexmap_art_test_group();
    hexmap_building_test_group();
    hexmap_geometry_test_group();
    image_io_test_group();
    image_resize_test_group();
    image_test_group();
    index_table_test_group();
    iterator_test_group();
    linear_map_test_group();
    log_scale_arithmetic_test_group();
    log_scale_basics_test_group();
    log_scale_formatting_test_group();
    log_scale_nested_test_group();
    log_scale_parsing_test_group();
    markov_test_group();
    markup_test_group();
    maths_arithmetic_functions_test_group();
    maths_literals_test_group();
    maths_properties_test_group();
    maths_special_functions_test_group();
    matrix_test_group();
    meta_test_group();
    mirror_map_test_group();
    mp_integer_rational_arithmetic_test_group();
    mp_integer_rational_conversion_test_group();
    mp_integer_sign_agnostic_test_group();
    mp_integer_signed_arithmetic_test_group();
    mp_integer_signed_conversion_test_group();
    mp_integer_unsigned_arithmetic_test_group();
    mp_integer_unsigned_conversion_test_group();
    multi_array_test_group();
    name_test_group();
    named_mutex_test_group();
    noise_test_group();
    options_anonymous_test_group();
    options_boolean_test_group();
    options_container_test_group();
    options_enumeration_test_group();
    options_help_test_group();
    options_mutual_exclusion_test_group();
    options_parsing_test_group();
    options_path_test_group();
    options_pattern_match_test_group();
    options_required_test_group();
    options_traits_test_group();
    options_types_test_group();
    path_directory_test_group();
    path_file_system_test_group();
    path_name_test_group();
    prime_fixed_binary_test_group();
    prime_int_factors_test_group();
    prime_int_next_prev_test_group();
    prime_int_test_group();
    prime_mp_integer_test_group();
    probability_arithmetic_test_group();
    probability_special_functions_test_group();
    probability_values_test_group();
    progress_test_group();
    projection_test_group();
    quaternion_test_group();
    random_adapter_test_group();
    random_algorithm_test_group();
    random_concept_test_group();
    random_continuous_test_group();
    random_discrete_test_group();
    random_lcg_test_group();
    random_non_arithmetic_test_group();
    random_pcg_test_group();
    random_seed_test_group();
    random_spatial_test_group();
    random_squirrel_test_group();
    random_xoshiro_test_group();
    rational_test_group();
    regex_basic_test_group();
    regex_compile_flags_test_group();
    regex_grep_test_group();
    regex_match_test_group();
    regex_replace_test_group();
    regex_runtime_flags_test_group();
    resource_test_group();
    root_finding_test_group();
    spatial_index_test_group();
    spectrum_formatting_test_group();
    spectrum_parsing_test_group();
    spectrum_property_test_group();
    sqlite_test_group();
    stack_test_group();
    statistics_test_group();
    stdio_test_group();
    string_casing_test_group();
    string_character_test_group();
    string_formatting_test_group();
    string_literals_test_group();
    string_manipulation_test_group();
    string_parsing_test_group();
    string_query_test_group();
    string_type_test_group();
    string_view_test_group();
    string_wrapping_test_group();
    superposition_test_group();
    table_test_group();
    terminal_test_group();
    text_gen_test_group();
    thread_pool_test_group();
    thread_test_group();
    time_test_group();
    topological_order_test_group();
    transform_test_group();
    unicode_normalization_test_group();
    unicode_test_group();
    uri_general_test_group();
    uri_schemes_test_group();
    uuid_test_group();
    vector_test_group();
    web_client_test_group();
    xml_construction_test_group();
    xml_functions_test_group();
    xml_options_test_group();
    xml_parsing_test_group();

    return Crow::UnitTest::end_tests();

}
