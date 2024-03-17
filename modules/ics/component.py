# from typing import Dict, List, TypeVar, Union


from ics.contentline import Container

# ComponentType = TypeVar("ComponentType", bound="Component")
# ComponentExtraParams = Dict[str, Union[ExtraParams, List[ExtraParams]]]


class Component:
    NAME = "ABSTRACT-COMPONENT"
    SUBTYPES = []

    def __init__(self):
        self_EXTRA_ATTR_NAME = 'extra' # Rename this if necessary to avoid conflicts
        self.EXTRA_PARAMS_ATTR_NAME = 'extra_params' # Rename this if necessary to avoid conflicts
        self.__dict__[self_EXTRA_ATTR_NAME] = Container(self.NAME)
        self.__dict__[self.EXTRA_PARAMS_ATTR_NAME] = {}
        Component.SUBTYPES.append(self.__class__)

    @classmethod
    def from_container(cls, container, context=None):
        raise NotImplementedError("from_container must be implemented by subclasses")

    def populate(self, container, context=None):
        raise NotImplementedError("populate must be implemented by subclasses")

    def to_container(self, context=None):
        raise NotImplementedError("to_container must be implemented by subclasses")

    def serialize(self, context=None):
        container = self.to_container(context)
        return container.serialize() if hasattr(container, 'serialize') else str(container)

    def strip_extras(self, all_extras=False, extra_properties=None, extra_params=None, property_merging=None):
        if extra_properties is None:
            extra_properties = all_extras
        if extra_params is None:
            extra_params = all_extras
        if property_merging is None:
            property_merging = all_extras
        if not any([extra_properties, extra_params, property_merging]):
            raise ValueError("need to strip at least one thing")
        if extra_properties:
            self.__dict__[self_EXTRA_ATTR_NAME].clear()
        if extra_params:
            self.__dict__[self.EXTRA_PARAMS_ATTR_NAME].clear()
        elif property_merging:
            for val in self.__dict__[self.EXTRA_PARAMS_ATTR_NAME].values():
                if not isinstance(val, list):
                    continue
                for v in val:
                    v.pop("__merge_next", None)

    def clone(self):
        from copy import deepcopy
        # Depending on what deep copy does in MicroPython, this may have unintended side effects
        return deepcopy(self)