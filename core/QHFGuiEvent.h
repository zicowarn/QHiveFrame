#ifndef QHFGUIEVENT_H
#define QHFGUIEVENT_H

#include <QHash>

namespace QHF {
    /**
     * @brief Event type (see description)
     * @details Identifies Event type
     * @note 枚举value必须从 0 开始, 且必须连续, 在此基础top增加value即可
     */
    enum class EventType : std::uint8_t {
        NONE,
        TEST_EVENT1,
        TEST_EVENT2,
        DEBUG_EVENT,
        OPEN_LOD,
        OPEN_PROFILE
    };

    /**
     * @brief  (see description)
     * @details (see source)Event type
     */
    namespace EventNameUtils {
        static const QHash<QString, EventType> eventNameMap = {
            {"TestEvent1", EventType::TEST_EVENT1},
            {"TestEvent2", EventType::TEST_EVENT2},
            {"DebugEvent", EventType::DEBUG_EVENT},
            {"OpenLOD", EventType::OPEN_LOD},
            {"OpenProfile", EventType::OPEN_PROFILE}};

        /**
         * @brief  (see description)
         * @param evt Event type
         * @return string*/
        inline QString toString(EventType evt)
        {
            return eventNameMap.key(evt);
        }

        /**
         * @brief  (see description)
         * @param str string* @return Event type
         */
        inline EventType fromString(const QString& str) {
            return eventNameMap.value(str, EventType::NONE);
        }
    } // namespace EventNameUtils
} // namespace QHF

#endif // HGUIEVENT_H
