/* A GTK+ wrapper around the cainteoir::tts::parameter class.
 *
 * Copyright (C) 2015 Reece H. Dunn
 *
 * This file is part of cainteoir-gtk.
 *
 * cainteoir-gtk is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cainteoir-gtk is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cainteoir-gtk.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CAINTEOIR_GTK_CAINTEOIR_SPEECH_PARAMETER_PRIVATE_H
#define CAINTEOIR_GTK_CAINTEOIR_SPEECH_PARAMETER_PRIVATE_H

#include <cainteoir/engines.hpp>

CainteoirSpeechParameter *             cainteoir_speech_parameter_new(const std::shared_ptr<cainteoir::tts::parameter> &parameter);

#endif
