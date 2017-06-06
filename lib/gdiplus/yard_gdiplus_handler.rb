# frozen_string_literal: true

# @private
class GdiplusEnumHandler < YARD::Handlers::C::Base
  MATCH_ENUM_1 = /define_enumint\((\w+), \w+, "(\w+)", (\w+)\)/
  MATCH_ENUM_2 = /gdip_enum_define<[^>]+>\((\w+), \w+, "(\w+)"/
  
  handles MATCH_ENUM_1
  handles MATCH_ENUM_2
  statement_class BodyStatement

  process do
    statement.source.scan(MATCH_ENUM_1) do |klass, const_name, value|
      handle_constants("const", klass, const_name, value)
      handle_enum_method(klass, const_name)
    end
    statement.source.scan(MATCH_ENUM_2) do |var_name, const_name|
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

# @private
class GdiplusAttrHandler < YARD::Handlers::C::Base
  MATCH_ATTR_R4 = /ATTR_R\((\w+), (\w+), (\w+), (\w+)\)/
  MATCH_ATTR_R5 = /ATTR_R\((\w+), (\w+), (\w+), (\w+), (\w+)\)/
  MATCH_ATTR_R_Q = /ATTR_R_Q\((\w+), (\w+), (\w+), (\w+)\)/
  MATCH_ATTR_RW = /ATTR_RW\((\w+), (\w+), (\w+), (\w+)\)/
  
  handles MATCH_ATTR_R4
  handles MATCH_ATTR_R5
  handles MATCH_ATTR_R_Q
  handles MATCH_ATTR_RW
  statement_class BodyStatement

  process do
    statement.source.scan(MATCH_ATTR_R4) do |klass, name1, name2, prefix|
      handle_method(:instance, klass, name1, "gdip_#{prefix}_get_#{name2}")
      handle_attribute(klass, name1, 1, 0)
      handle_alias(klass, name2, name1)
    end
    statement.source.scan(MATCH_ATTR_R5) do |klass, name1, name2, prefix, name3|
      handle_attribute(klass, name1, 1, 0)
      handle_alias(klass, name2, name1)
    end
    statement.source.scan(MATCH_ATTR_R_Q) do |klass, name1, name2, prefix|
      handle_method(:instance, klass, name1, "gdip_#{prefix}_get_#{name2}")
      handle_attribute(klass, name1, 1, 0)
      handle_alias(klass, "#{name2}?", name1)
    end
    statement.source.scan(MATCH_ATTR_RW) do |klass, name1, name2, prefix|
      handle_method(:instance, klass, name1, "gdip_#{prefix}_get_#{name2}")
      handle_attribute(klass, name1, 1, 1)
      handle_alias(klass, name2, name1)
    end
    
  end
  
end

# @private
module YARD
  module Handlers
    module C
      module HandlerMethods
        def handle_method(scope, var_name, name, func_name, _source_file = nil)
          #p [:metho, scope, var_name, name, func_name, _source_file]
          visibility = :public
          case scope
          when "singleton_method"; scope = :class
          when "module_function"; scope = :module
          when "private_method"; scope = :instance; visibility = :private
          else; scope = :instance
          end

          namespace = namespace_for_variable(var_name)

          # Is this method being defined on a core Ruby class or module?
          if namespace.is_a?(Proxy)
            if var_name =~ /^rb_c(\w+)/ && YARD::CodeObjects::BUILTIN_CLASSES.include?($1)
              namespace = namespaces[var_name] = YARD::CodeObjects::ClassObject.new(:root, $1)
            elsif var_name =~ /^rb_m(\w+)/ && YARD::CodeObjects::BUILTIN_MODULES.include?($1)
              namespace = namespaces[var_name] = YARD::CodeObjects::ModuleObject.new(:root, $1)
            end
          end

          return if namespace.nil? # XXX: raise UndocumentableError might be too noisy.
          register MethodObject.new(namespace, name, scope) do |obj|
            register_visibility(obj, visibility)
            find_method_body(obj, func_name)
            obj.explicit = true
            if name.end_with?("?") && !(obj.tag(:return) || (obj.tag(:overload) && obj.tag(:overload).tag(:return)))
              obj.add_tag(Tags::Tag.new(:return, '', 'Boolean'))
            end
          end
        end # handle_method
        
      end
    end
  end
end

