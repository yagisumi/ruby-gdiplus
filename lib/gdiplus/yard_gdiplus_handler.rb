# frozen_string_literal: true

# @private
class GdiplusEnumHandler < YARD::Handlers::C::Base
  MATCH1 = /define_enumint\((\w+), &\w+, "(\w+)", (\w+)\)/
  MATCH2 = /gdip_enum_define<[^>]+>\((\w+), &\w+, "(\w+)"/
  
  handles MATCH1
  handles MATCH2
  statement_class BodyStatement

  process do
    statement.source.scan(MATCH1) do |tname, const_name, value|
      handle_constants("const", "c#{tname}", const_name, value)
      handle_enum_method("c#{tname}", const_name)
    end
    statement.source.scan(MATCH2) do |var_name, const_name|
      klass = var_name.sub(/^c/, '')
      value = "#{klass}#{const_name}"
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
