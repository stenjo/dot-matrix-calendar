import heapq

from datetime import date, datetime, timedelta
from utils import ceil_datetime_to_midnight, ensure_datetime, floor_datetime_to_midnight

class Timeline:
    """
    `Timeline`s allow iterating all event from a `Calendar` in chronological order, optionally also filtering events
    according to their timestamps.
    """


    def __normalize_datetime(self, instant):
        """
        Create a normalized datetime instance for the given instance.
        """
        if self._normalization:
            instant = self._normalization.normalize(instant)
        return instant

    def __normalize_timespan(
        self, start, stop = None
    ):
        """
        Create a normalized timespan between `start` and `stop`.
        Alternatively, this method can be called directly with a single timespan as parameter.
        """
        # if isinstance(start, Timespan):
        #     if stop is not None:
        #         raise ValueError("can't specify a Timespan and an additional stop time")
        timespan = start
        # else:
        #     timespan = Timespan(ensure_datetime(start), ensure_datetime(stop))
        # if self._normalization:
        #     timespan = self._normalization.normalize(timespan)
        return timespan

    def iterator(self):
        """
        Iterates on every event from the :class:`ics.icalendar.Calendar` in chronological order

        Note:
            - chronological order is defined by the comparison operators in :class:`ics.timespan.Timespan`
            - Events with no `begin` will not appear here. (To list all events in a `Calendar` use `Calendar.events`)
        """
        # Using a heap is faster than sorting if the number of events (n) is
        # much bigger than the number of events we extract from the iterator (k).
        # Complexity: O(n + k log n).
        heap = (
            (self.__normalize_timespan(e.timespan), e) for e in self._calendar.events
        )
        heap = [t for t in heap if t[0]]
        heapq.heapify(heap)
        while heap:
            yield heapq.heappop(heap)

    def __iter__(self):
        """
        Iterates on every event from the :class:`ics.icalendar.Calendar` in chronological order

        Note:
            - chronological order is defined by the comparison operators in :class:`ics.timespan.Timespan`
            - Events with no `begin` will not appear here. (To list all events in a `Calendar` use `Calendar.events`)
        """
        for _, e in self.iterator():
            yield e

    def included(
        self, start, stop = None
    ) :
        """
        Iterates (in chronological order) over every event that is included in the timespan between `start` and `stop`.
        Alternatively, this method can be called directly with a single timespan as parameter.
        """
        query = self.__normalize_timespan(start, stop)
        for timespan, event in self.iterator():
            if timespan.is_included_in(query):
                yield event

    def overlapping(
        self, start, stop = None
    ) :
        """
        Iterates (in chronological order) over every event that has an intersection with the timespan between `start` and `stop`.
        Alternatively, this method can be called directly with a single timespan as parameter.
        """
        query = self.__normalize_timespan(start, stop)
        for timespan, event in self.iterator():
            if timespan.intersects(query):
                yield event

    def start_after(self, instant):
        """
        Iterates (in chronological order) on every event from the :class:`ics.icalendar.Calendar` in chronological order.
        The first event of the iteration has a starting date greater (later) than `instant`.
        """
        instant = self.__normalize_datetime(instant)
        for timespan, event in self.iterator():
            if timespan.begin_time is not None and timespan.begin_time > instant:
                yield event

    def at(self, instant) :
        """
        Iterates (in chronological order) over all events that are occuring during `instant`.
        """
        instant = self.__normalize_datetime(instant)
        for timespan, event in self.iterator():
            if timespan.includes(instant):
                yield event

    def on(self, instant, strict: bool = False):
        """
        Iterates (in chronological order) over all events that occurs on `day`.

        :param strict: if True events will be returned only if they are strictly *included* in `day`
        """
        begin = floor_datetime_to_midnight(ensure_datetime(instant))
        end = ceil_datetime_to_midnight(ensure_datetime(instant) + timedelta(seconds=1))
        query = self.__normalize_timespan(begin, end)
        if strict:
            return self.included(query)
        else:
            return self.overlapping(query)

    def today(self, strict: bool = False):
        """
        Iterates (in chronological order) over all events that occurs today.

        :param strict: if True events will be returned only if they are strictly *included* in `day`
        """
        return self.on(date.today(), strict=strict)

    def now(self):
        """
        Iterates (in chronological order) over all events that occur right now.
        """
        return self.at(datetime.utcnow())
