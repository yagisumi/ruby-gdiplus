# frozen_string_literal: true

# @private
class GdiplusEnumHandler < YARD::Handlers::C::Base
  MATCH = /gdip_enum_int_define\((\w+),\s*\w+,\s*"(\w+)",\s*(\w+)\)/
  handles MATCH
  statement_class BodyStatement

  process do
    statement.source.scan(MATCH) do |var_name, const_name, value|
      handle_constants("const", var_name, const_name, value)
      handle_enum_method(var_name, const_name)
    end
  end
  
  def handle_enum_method(var_name, name)
    namespace = namespace_for_variable(var_name)
    register MethodObject.new(namespace, name, :class) do |obj|
      register_visibility(obj, :public)
      
      klass = var_name.sub(/^c/, '')
      obj.add_tag(YARD::Tags::Tag.new(:return, "{Gdiplus::#{klass}::#{name}}", klass))
      obj.explicit = true
    end
  end
end
